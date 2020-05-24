
class Entity {
public:
	float x,y;
	Engine::Sprite sprite;
	Entity();
	Entity(float& x_in, float& y_in);
};


class Projectile : public Entity{
public:
	constexpr static float step = 1.0; //to incorporate speed
	bool direction_up;
	void collide();
	Engine::Sprite sprite;
	Projectile();
	Projectile(float& x_in, float& y_in);
};

/// Inherited classs rocket

class Rocket : public Projectile {

public:
	const bool direction_up = true;
	const static Engine::Sprite sprite = Engine::Sprite::Rocket;
	// next line will call Projectile constructor
	Rocket();
	Rocket(float& x_in, float& y_in);
	void move();
	
};

class Bomb : public Projectile {
public:
	const bool direction_up = false;
	const static Engine::Sprite sprite = Engine::Sprite::Bomb;
	// next line will call Projectile constructor
	Bomb();
	Bomb(float& x_in, float& y_in);
	void move();
};

class Ship : public Entity {

public:
	float y = Engine::CanvasHeight - Engine::SpriteSize;
	bool alive = true;
	int lives = 3;
	void Shoot();
	void Damage();
	Ship();
	Ship(float& x_in, float& y_in);
	const static Engine::Sprite sprite = Engine::Sprite::Player;

};


///////////ALIEN CLASS /////////////////////
class Alien : public Entity{
public:
	const static Engine::Sprite sprite = Engine::Sprite::Enemy1;
	// constructor & destructor

	bool alive = true;	
	Alien();
	Alien(float& x_in, float& y_in);
//functions
	void SideMove();
	void Shoot(std::vector<Bomb>& bombs);
	void Die();
};











