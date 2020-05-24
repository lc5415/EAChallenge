#include "Engine.h"
#include "iostream"
#include "entities.h"
#include <vector>
#include <string>
#include <algorithm>

Entity::Entity(){}
Entity::~Entity(){}
Entity::Entity(float& x_in, float& y_in): x(x_in), y(y_in){}

Projectile::Projectile():Entity(){}
Projectile::Projectile(float& x_in, float& y_in):Entity(x_in,y_in){}

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


void Alien::Shoot(std::vector<Bomb>& bombs){ 
	// instead of define a rate of shooting
	// I only shoot one of every 100 times. Couldd change for 
	// a time based shooting
	if (rand()%2 == 1) bombs.emplace_back(x, y);
};

void checkHit(Projectile& p, Entity& e, Ship& player)
{/*This check hits between rockets and aliens and add score to the player*/
	const int size = Engine::SpriteSize;
	if (p.x + size > e.x and
		 e.x+size > p.x and 
		 e.y+size > p.y and
		 p.y+size > e.y)
	{
		p.active = false;
		e.active = false;
		player.score++;
		player.score_msg = std::to_string(player.score);
	}

}

void checkHit(Projectile& p, Ship& s)
{/*This check hits between the bombs and the player and updates the player
life*/
	const int size = Engine::SpriteSize;
	if (p.x + size > s.x and
		 s.x+size > p.x and 
		 s.y+size > p.y and
		 p.y+size > s.y)
	{
		s.lives--;
		s.lives_msg = std::to_string(s.lives);
		p.active = false;
	}
}


class Fleet {
public:
	int width, height, count;
	std::vector<Alien> aliens;
	bool flow_right = true;


	Fleet(int w_in, int h_in);
	void Down();
	void Dynamics(Engine& engine,double& timestamp,
		double& time_at_lastbomb,double& delay_btwn_bombs, 
		std::vector<Bomb>& bombs, std::vector<Rocket>& rockets,
		Ship& player);
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
	std::vector<Bomb>& bombs,
	std::vector<Rocket>& rockets, 
	Ship& player)
{

	for (Alien& a: aliens)
	{	
		if (a.active)
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
			if (a.active and
				timestamp-time_at_lastbomb>delay_btwn_bombs)
			{
				a.Shoot(bombs);
				time_at_lastbomb = timestamp;
			}

		//check collisions with rockets
			for (Rocket& r: rockets)
			{	
				if (r.active) checkHit(r, a, player);
			}
		}

		count -= !(a.active); // update count by substracting 
		// no longer active aliens
		
	}


};




/////////MAIN BODY///////////
void EngineMain()
{
	Engine engine;
	bool start = false;

	Ship player;
	player.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;

	double time_at_lastshot = engine.getStopwatchElapsedSeconds();
	double time_at_lastbomb = engine.getStopwatchElapsedSeconds();
	double delay_btwn_shots = 0.5; //half a second
	double delay_btwn_bombs = 0.5;

	const int w_aliens = 8; // Number of rows with aliens
	const int h_aliens = 4; // Number of columns with aliens
	Fleet enemies(w_aliens, h_aliens);

	std::vector<Rocket> rocks; //rocket (empty) vector
	std::vector<Bomb> bombs; //bomb (empty) vector

	std::string l_msg("LIVES: "), s_msg("SCORE: ");
	int l_msg_x = 1, l_msg_y = 1;
	int s_msg_x = 1, s_msg_y = 1+Engine::SpriteSize;

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
			
		// Move aliens if player is active
			if (player.active)
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
					time_at_lastshot = timestamp;
				}
				// Rockets display+movement
				for (Rocket& r: rocks) // reference to avoid copying
				{
					if (r.active){
						engine.drawSprite(
							r.sprite, 
							r.x, r.y);
						r.move();
					}
				}

				enemies.Dynamics(engine,
					timestamp,
					time_at_lastbomb,
					delay_btwn_bombs,
					bombs,
					rocks,
					player);

				// bombs display + movement
				for (Bomb& b: bombs) // reference to avoid copying
				{	
					engine.drawSprite(
						b.sprite, 
						b.x, b.y);
					b.move();

					//check for collisions with the player
					checkHit(b, player);
				}

				//print lives
				engine.drawText((l_msg+player.lives_msg).c_str(),
					l_msg_x, l_msg_y); 
				//print score
				engine.drawText((s_msg+player.score_msg).c_str(),
					s_msg_x, s_msg_y);

				/*

				 DISSAPEARANCE MODULE
				
				Here we are using erase and remove_if combined
				to effectively clear out the memory of objects that are no
				longer active. This is of particular importance for
				the projectiles given that very large numbers of these could
				be made.

				remove_if just clears the memory but the container
				size remains the same, the implementation below is equivalent
				to use std::experimental::erase_if()
				*/

				//clear out rockets
				rocks.erase(
					std::remove_if(rocks.begin(), rocks.end(),
						[](Rocket& r){
							bool dissapear = !(r.active) or (r.y<0);
							return dissapear;
						}), rocks.end());

				//clear out aliens
				enemies.aliens.erase(
					std::remove_if(enemies.aliens.begin(),
						enemies.aliens.end(),
						[](Alien& a){

							bool dissapear = !(a.active) or
							(a.y>Engine::CanvasHeight);
							return dissapear;

						}), enemies.aliens.end()); 

				//clear out bombs
				bombs.erase(
					std::remove_if(bombs.begin(),
						bombs.end(),
						[](Bomb& b){

							bool dissapear = !(b.active) or
							(b.y>Engine::CanvasHeight-Engine::SpriteSize);
							return dissapear;

						}), bombs.end()); 

				if (player.lives==0) player.active = false;



			} else { //if player not active
				const char message[] = "YOU SUCK!";
				engine.drawText(
					message, 
					(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
					(Engine::CanvasHeight - Engine::FontRowHeight) / 2); 

			}

		}
	}


}


