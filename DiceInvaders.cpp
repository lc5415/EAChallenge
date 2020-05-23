#include "Engine.h"
#include "iostream"
#include <stdint.h>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

class Projectile {
public:
	int x, y;
	bool direction_up;
	void collide();
	//y = 0 is at the top of the screen
	void move(){if (direction_up) {y--;} else {y++;}};
	Engine::Sprite sprite;
	Projectile(); // Non-parametric constructor
	Projectile(int& x_in, int& y_in): x(x_in), y(y_in){}
	~Projectile();
};

void Projectile::collide()
{

}

/// Inherited classs rocket

class Rocket : public Projectile {
public:
	bool direction_up = true;
	Engine::Sprite sprite = Engine::Sprite::Rocket;
	// next line will call Projectile constructor
	Rocket(int x_in, int y_in):Projectile(x_in,y_in){}
	~Rocket();
};

class Bomb : public Projectile {
public:
	bool direction_up = false;
	Engine::Sprite sprite = Engine::Sprite::Bomb;
	// next line will call Projectile constructor
	Bomb(int x_in, int y_in): Projectile(x_in,y_in){}
	~Bomb();
};


/////////SHIP CLASS /////////////////////////////////
class Ship {

public:
	int x;
	bool alive = true;
	int lives = 3;
	void Shoot();
	void Damage();

};


///////////ALIEN CLASS /////////////////////
class Alien {
public:
// by being static these apply to all objects of class alien
	static bool flow_right;
	static int count;
static int y; //it makes sense in the 1 row scenario as 
// all aliens lie on the same row

// constructor & destructor
Alien();
~Alien();


//functions
int x;			
bool alive = true;	

//functions
void Spawn(Engine engine, int entry_x, int entry_y);
void SideMove();
void Bomb();
void Die();


private:

	static void Down();
};


// Constructor definition
Alien::Alien(){
	count++;
}

Alien::~Alien(){
	count--;
}


// functions
void Alien::Spawn(Engine engine, int entry_x, int entry_y)
{
	engine.drawSprite(
		Engine::Sprite::Enemy1, 
		entry_x, entry_y);
}

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

void Alien::Bomb(){};
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

	int x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
	int y_player = Engine::CanvasHeight - Engine::SpriteSize;

	Ship player;
	
	int alienCount = 0;
	int x_entry_alien = 0, y_entry_alien = Engine::CanvasHeight;


	//get max now for when user is prompted so that he does not input
	// more aliens that what can be put
	int max_aliens = Engine::CanvasWidth/Engine::SpriteSize;

	const int n_aliens = 5; //can prompt this later
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
		/* INITIAL WINDOW:
		Wait until user presses space to start the game */ 

		if (not start){
			const uint8_t* keyboard = SDL_GetKeyboardState(0);
			const char message[] = "PRESS SPACE KEY TO START";
			engine.drawText(
				message, 
				(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
				(Engine::CanvasHeight - Engine::FontRowHeight) / 2); 

			if (keyboard[SDL_SCANCODE_SPACE]) start = true;
		} else {

		/* START GAME 
		This implementation is good because the no counter
		or other background process starts until the user presses space*/

			double timestamp = engine.getStopwatchElapsedSeconds();
			(void)timestamp;
			iter++;

			Engine::PlayerInput keys = engine.getPlayerInput();
			if (keys.left)  --x;
			if (keys.right) ++x;

			engine.drawSprite(
				Engine::Sprite::Player, 
				x, y_player);

		// Move aliens if player is alive
			if (player.alive)
			{	
				// shooting
				if (keys.fire){
					rocks.emplace_back(x, y_player); 
					// equiv to .push_back(Rocket(x, y_player))
					rocket_count++;
				}
				// Rockets movement
				for (Rocket& r: rocks) // reference to avoid copying
				{
					engine.drawSprite(
						r.sprite, 
						r.x, r.y);
					r.move();
				}


				// aliens movement
				for (Alien &unit: aliens){
					// Draw aliens
					engine.drawSprite(
					Engine::Sprite::Enemy1, 
					unit.x,
					unit.y);
					// Move aliens
					unit.SideMove();
				}


			}

		}
	}


}


