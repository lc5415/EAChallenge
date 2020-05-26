#include "iostream"
#include "Engine.h"
#include <vector>
#include <algorithm>
#include <string>


class Entity {
	/*
	All objects in the game (ship, aliens, rocket and bombs)
	will be children of the Entity class
	*/

public:

	//variables
	int x,y; // position
	int width = Engine::SpriteSize; 
	int height = Engine::SpriteSize;
	Engine::Sprite sprite; // sprite
	bool active = true; // aka alive
	
	// Constructors & Destructor
	Entity();
	~Entity();
	Entity(int& x_in, int& y_in);

};


class Projectile : public Entity{
	/*
	Bombs and rockets will inherit from the Projectile class
	*/
public:

	int step = 1; // step size at each update

	// constructors
	Projectile();
	Projectile(int& x_in, int& y_in);
	~Projectile();
};

class Rocket : public Projectile {

public:
	// set sprite to Rocket sprite
	Engine::Sprite sprite = Engine::Sprite::Rocket;

	// {Con/Des}tructors
	Rocket();
	Rocket(int& x_in, int& y_in);
	~Rocket();

	// move function (step up (y--;))
	void move();
	
};

class Bomb : public Projectile {
public:
	// set sprite to Rocket sprite
	Engine::Sprite sprite = Engine::Sprite::Bomb;

	// {Con/De}structors
	Bomb();
	Bomb(int& x_in, int& y_in);
	~Bomb();

	// move function (step down (y++;))
	void move();
};

class Ship : public Entity {

public:
	// attributes

	// y will always be the same for the ship so set it here
	int y = Engine::CanvasHeight - Engine::SpriteSize;

	// sprite
	const static Engine::Sprite sprite = Engine::Sprite::Player;

	// number of lives
	int lives = 3; 

	// lives msg for dashboard
	std::string lives_msg = std::to_string(lives); 

	// functions
	void Shoot(std::vector<Rocket>& r);

	// {Con/De}structors
	Ship();
	Ship(int& x_in, int& y_in);
	~Ship();

};


///////////ALIEN CLASS /////////////////////
class Alien : public Entity{
public:
	// attributes
	
	// sprite
	const static Engine::Sprite sprite = Engine::Sprite::Enemy1;
	
	//functions
	bool Shoot(std::vector<Bomb>& bombs);

	// {Con/De}structors
	Alien();
	Alien(int& x_in, int& y_in);
	~Alien();
	

};











