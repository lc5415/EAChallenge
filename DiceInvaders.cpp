#include "Engine.h"
#include "iostream"
#include "entities.h"
#include <vector>
#include <algorithm>



Projectile::Projectile(int& x_in, int& y_in): x(x_in), y(y_in){}

void Projectile::collide()
{

}

Rocket::Rocket(int x_in, int y_in):Projectile(x_in,y_in){}
Bomb::Bomb(int x_in, int y_in): Projectile(x_in,y_in){}

void Rocket::move(){y-=step;}

void Bomb::move(){y+=step;}




// Constructor definition
Alien::Alien(){
	count++;
}

Alien::~Alien(){
	count--;
}


// functions

void Alien::SideMove(){
/*
Aliens start in top-left corner and move to the right,
when they any alien reaches the right, all aliens go down
next when any alien touches the edge, all aliens go down
*/

	if (flow_right){
		x++;
	} else {
		x--;
	}

	// this is somewhat obscure because it's not in main
	if ((flow_right and x == Engine::CanvasWidth-Engine::SpriteSize) or 
		(not flow_right and x == 0))
	{
		flow_right = not flow_right;
		Down();
	}

}

void Alien::Shoot(std::vector<Bomb>& bombs){ 
	// instead of define a rate of shooting
	// I only shoot one of every 100 times. Couldd change for 
	// a time based shooting
	if (rand()%2 == 1) bombs.emplace_back(x, y);

};
void Alien::Die(){};
void Alien::Down(){
// this is also static as we
// want it to affect all units
	y += Engine::SpriteSize;
}

int Alien::y = 0; // initialise y for all aliens
bool Alien::flow_right = true;
int Alien::count = 0;

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

	const int n_aliens = 8; //can prompt this later
	Alien aliens[n_aliens]; //aliens vector

	std::vector<Rocket> rocks; //rocket (empty) vector
	int rocket_count = 0;
	std::vector<Bomb> bombs; //bomb (empty) vector
	int bomb_count = 0;

	// initialise aliens
	for (int i = 0; i<n_aliens; i++){
		aliens[i].x = Engine::SpriteSize*(i);
	}


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


				// Aliens dynamics
				for (Alien &unit: aliens)
				{
					// Draw aliens
					engine.drawSprite(
					unit.sprite, 
					unit.x,
					unit.y);

					// Move aliens
					unit.SideMove();

					//bombs
					if (unit.alive and
					 timestamp-time_at_lastbomb>delay_btwn_bombs)
					{
					 	unit.Shoot(bombs);
						time_at_lastbomb = timestamp;
					}
				}

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


