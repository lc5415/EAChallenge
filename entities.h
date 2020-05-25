#include "iostream"
#include "Engine.h"
#include <vector>
#include <algorithm>
#include <string>


class Entity {
public:
	float x,y;
	int width, height;
	Engine::Sprite sprite;
	bool active = true;
	Entity();
	~Entity();
	Entity(float& x_in, float& y_in);

	friend void checkHit();
};


class Projectile : public Entity{
public:
	constexpr static float step = 1.0; //to incorporate speed
	bool direction_up;
	Engine::Sprite sprite;
	Projectile();
	Projectile(float& x_in, float& y_in);
	~Projectile();
};

/// Inherited classs rocket

class Rocket : public Projectile {

public:
	bool direction_up = true;
	const static Engine::Sprite sprite = Engine::Sprite::Rocket;
	// next line will call Projectile constructor
	Rocket();
	Rocket(float& x_in, float& y_in);
	~Rocket();
	void move();
	
};

class Bomb : public Projectile {
public:
	bool direction_up = false;
	const static Engine::Sprite sprite = Engine::Sprite::Bomb;
	// next line will call Projectile constructor
	Bomb();
	Bomb(float& x_in, float& y_in);
	~Bomb();
	void move();
};

class Ship : public Entity {

public:
	float y = Engine::CanvasHeight - Engine::SpriteSize;
	int lives = 3;
	std::string lives_msg = std::to_string(lives);
	void Shoot();
	void Damage();
	Ship();
	Ship(float& x_in, float& y_in);
	~Ship();
	const static Engine::Sprite sprite = Engine::Sprite::Player;

};


///////////ALIEN CLASS /////////////////////
class Alien : public Entity{
public:
	const static Engine::Sprite sprite = Engine::Sprite::Enemy1;
	// constructor & destructor
	Alien();
	Alien(float& x_in, float& y_in);
	~Alien();
//functions
	bool Shoot(std::vector<Bomb>& bombs);
};











