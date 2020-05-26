//entities.cpp
#include "entities.h"

// Entitiy class
Entity::Entity(){}

Entity::~Entity(){}

Entity::Entity(int& x_in, int& y_in): x(x_in), y(y_in){}


// Projectile class
Projectile::Projectile():Entity(){}

Projectile::~Projectile(){}

Projectile::Projectile(int& x_in, int& y_in):Entity(x_in,y_in){}


// Rocket class
Rocket::Rocket():Projectile(){}

Rocket::~Rocket(){}

Rocket::Rocket(int& x_in, int& y_in):Projectile(x_in,y_in){}

void Rocket::move(){y-=step;}


// Bomb class

Bomb::Bomb():Projectile(){}

Bomb::~Bomb(){}

Bomb::Bomb(int& x_in, int& y_in): Projectile(x_in,y_in){
	y += Engine::SpriteSize;
}

void Bomb::move(){y+=step;}


// Ship class

Ship::Ship():Entity(){}

Ship::~Ship(){}

Ship::Ship(int& x_in, int& y_in):Entity(x_in, y_in){}

void Ship::Shoot(std::vector<Rocket>& rocket){
	rockets.emplace_back(x, y);
};


// Alien class

Alien::Alien():Entity(){}

Alien::~Alien(){}

Alien::Alien(int& x_in, int& y_in):Entity(x_in, y_in){}

bool Alien::Shoot(std::vector<Bomb>& bombs)
{ 
	if (rand()%100 == 1){ // rand()%number is equivalent to the chance a bomb is shot
		//each time this function is called, because of the high frame rate, if the 
		// number is small one will basically get a waterfall of bombs
		bombs.emplace_back(x,y);
		return true;
	} 
	return false;
};



