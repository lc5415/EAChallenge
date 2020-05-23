class Projectile {
public:
	float x, y;
	static float step = 1; //to incorporate speed
	bool direction_up;
	void collide();
	Engine::Sprite sprite;
	Projectile(int& x_in, int& y_in);
};

/// Inherited classs rocket

class Rocket : public Projectile {

public:
	const bool direction_up = true;
	const static Engine::Sprite sprite = Engine::Sprite::Rocket;
	// next line will call Projectile constructor
	Rocket(int x_in, int y_in);
	void move();
	
};

class Bomb : public Projectile {
public:
	const bool direction_up = false;
	const static Engine::Sprite sprite = Engine::Sprite::Bomb;
	// next line will call Projectile constructor
	Bomb(int x_in, int y_in);
	void move();
};

class Ship {

public:
	int x;
	const int y = Engine::CanvasHeight - Engine::SpriteSize;
	bool alive = true;
	int lives = 3;
	void Shoot();
	void Damage();
	const static Engine::Sprite sprite = Engine::Sprite::Player;

};


///////////ALIEN CLASS /////////////////////
class Alien {
public:
// by being static these apply to all objects of class alien
	static bool flow_right;
	static int count;
	const static Engine::Sprite sprite = Engine::Sprite::Enemy1;
	static int y; //it makes sense in the 1 row scenario as 
	// all aliens lie on the same row

	// constructor & destructor
	Alien();
	~Alien();


//functions
	int x;			
	bool alive = true;	

//functions
	void SideMove();
	void Shoot(std::vector<Bomb>& bombs);
	void Die();


private:

	static void Down();
};