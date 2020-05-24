#include "Engine.h"
#include "iostream"
#include "entities.h"
#include <vector>
#include <algorithm>

Entity::Entity(){}
Entity::Entity(float& x_in, float& y_in): x(x_in), y(y_in){}

Projectile::Projectile():Entity(){}
Projectile::Projectile(float& x_in, float& y_in):Entity(x_in,y_in){}

void Projectile::collide()
{

}

Rocket::Rocket():Projectile(){}
Rocket::Rocket(float& x_in, float& y_in):Projectile(x_in,y_in){}
Bomb::Bomb():Projectile(){}
Bomb::Bomb(float& x_in, float& y_in): Projectile(x_in,y_in){}

void Rocket::move(){y-=step;}

void Bomb::move(){y+=step;}

Ship::Ship():Entity(){}

Ship::Ship(float& x_in, float& y_in):Entity(x_in, y_in){}
// functions

Alien::Alien():Entity(){}
Alien::Alien(float& x_in, float& y_in):Entity(x_in, y_in){}

void Alien::SideMove(){
/*
Aliens start in top-left corner and move to the right,
when they any alien reaches the right, all aliens go down
next when any alien touches the edge, all aliens go down
*/




}

void Alien::Shoot(std::vector<Bomb>& bombs){ 
	// instead of define a rate of shooting
	// I only shoot one of every 100 times. Couldd change for 
	// a time based shooting
	if (rand()%2 == 1) bombs.emplace_back(x, y);
};
void Alien::Die(){};


class Fleet {
public:
	int width, height, count;
	std::vector<Alien> aliens;
	bool flow_right = true;


	Fleet(int w_in, int h_in);
	void Down();
	void Dynamics(Engine& engine,double& timestamp,
		double& time_at_lastbomb,double& delay_btwn_bombs, 
  		std::vector<Bomb>& bombs);
};

Fleet::Fleet(int w_in, int h_in):width(w_in), height(h_in){

	// allocate memory 
	count = w_in*h_in;	
	aliens.resize(count);

	//Give in initial position
	for (int j = 0; j<h_in; j++)
	{
		for (int i = 0; i<w_in;i++)
		{
			aliens[i+j*w_in].x = Engine::SpriteSize*i;
			aliens[i+j*w_in].y = Engine::SpriteSize*j;
		}
	}
}

void Fleet::Down(){
	for (Alien& a: aliens)
		a.y+=Engine::SpriteSize;
};

void Fleet::Dynamics(Engine& engine,
	double& timestamp,
 	double& time_at_lastbomb,
  	double& delay_btwn_bombs, 
 	std::vector<Bomb>& bombs)
{

	for (Alien& a: aliens)
	{	
		if (flow_right)
		{
			a.x++;
			// Check if unit hit right edge
			if (a.x == Engine::CanvasWidth-Engine::SpriteSize)
			{
				flow_right = not flow_right;
				Down();
			}
		} else {
			a.x--;
			// Check if unit hit right edge
			if (a.x == 0)
			{
				flow_right = not flow_right;
				Down();
			}
		}

		// display
		engine.drawSprite(
			a.sprite, 
			a.x, a.y);

		//bombs
		if (a.alive and
		 timestamp-time_at_lastbomb>delay_btwn_bombs)
		{
		 	a.Shoot(bombs);
			time_at_lastbomb = timestamp;
		}
	}


};

/////////MAIN BODY///////////
void EngineMain()
{
	Engine engine;
	bool start = false;

	Ship player;
	player.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;

	
	int alienCount = 0;
	int x_entry_alien = 0, y_entry_alien = Engine::CanvasHeight;

	double time_at_lastshot = engine.getStopwatchElapsedSeconds();
	double time_at_lastbomb = engine.getStopwatchElapsedSeconds();
	double delay_btwn_shots = 0.5; //half a second
	double delay_btwn_bombs = 0.5;

	//get max now for when user is prompted so that he does not input
	// more aliens that what can be put
	int max_aliens = Engine::CanvasWidth/Engine::SpriteSize;

	const int w_aliens = 8; // Number of rows with aliens
	const int h_aliens = 4; // Number of columns with aliens
	Fleet enemies(w_aliens, h_aliens);

	std::vector<Rocket> rocks; //rocket (empty) vector
	int rocket_count = 0;
	std::vector<Bomb> bombs; //bomb (empty) vector
	int bomb_count = 0;

	int iter = 0;
	while (engine.startFrame())
	{	

		/*
		Get player input as this is fundamental before the game 
		starts and during the game
		*/
		Engine::PlayerInput keys = engine.getPlayerInput();

		/* INITIAL WINDOW:
		Wait until user presses space to start the game */ 

		if (not start){
			const char message[] = "PRESS SPACE KEY TO START";
			engine.drawText(
				message, 
				(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
				(Engine::CanvasHeight - Engine::FontRowHeight) / 2); 

			if (keys.fire) start = true; // if hit space start game
		} else {

		/* START GAME 
		This implementation is good because the no counter
		or other background process starts until the user presses space*/

			double timestamp = engine.getStopwatchElapsedSeconds();
			(void)timestamp;
			iter++;
			
		// Move aliens if player is alive
			if (player.alive)
			{	
				// ship movement + display
				if (keys.left and player.x>0)  --player.x;
				if (keys.right and
				 player.x < Engine::CanvasWidth-Engine::SpriteSize) ++player.x;

				engine.drawSprite(
					player.sprite, 
					player.x, player.y);

				// ship shooting
				if (keys.fire and
				 timestamp-time_at_lastshot>delay_btwn_shots)
				{
					rocks.emplace_back(player.x, player.y); 
					// equiv to .push_back(Rocket(x, y_player))
					rocket_count++;
					time_at_lastshot = timestamp;
				}
				// Rockets display+movement
				for (Rocket& r: rocks) // reference to avoid copying
				{
					engine.drawSprite(
						r.sprite, 
						r.x, r.y);
					r.move();
				}

				enemies.Dynamics(engine,
					timestamp,
					time_at_lastbomb,
					delay_btwn_bombs,
					bombs);

				// bombs display + movement
				for (Bomb& b: bombs) // reference to avoid copying
				{
					engine.drawSprite(
						b.sprite, 
						b.x, b.y);
					b.move();
				}

				/* 

				COLLISION MODULE

				*/




				/*

				 DISSAPEARANCE MODULE

				*/


			}

		}
	}


}


