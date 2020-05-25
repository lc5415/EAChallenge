#include "iostream"
#include "Engine.h"
#include "entities.h"
#include <vector>
#include <algorithm>


struct Fleet {
	int width, height, count;
	std::vector<Alien> units;
	bool flow_right = true;

	Fleet();
	Fleet(int& w_in, int& h_in);

	void Down();

} ;

class Game {
public:
	// game metrics
	int round = 0;
	int iter = 0;
	int score = 0; 
	bool start = false;

	// time management metrics
	double timestamp;
	double time_at_lastshot;
	double time_at_lastbomb;
	double delay_btwn_shots = 0.5; //half a second
	double delay_btwn_bombs = 0.25; // make aliens shoot fast as twice as ship

	// message board
	std::string l_msg = "LIVES: ";
	std::string s_msg = "SCORE: ";
	std::string r_msg = "ROUND: ";
	const char idleMsg[25] = "PRESS SPACE KEY TO START";
	std::string score_msg = std::to_string(score);
	std::string round_msg = std::to_string(round);
	int l_msg_x = 1, l_msg_y = 1;
	int s_msg_x = 1, s_msg_y = 1+Engine::SpriteSize;
	int r_msg_x = 1, r_msg_y = 1+2*Engine::SpriteSize;

	// Engine and entities
	Ship player;
	Fleet aliens;
	std::vector<Rocket> rockets; //rocket (empty) vector
	std::vector<Bomb> bombs; //bomb (empty) vector
	const int spriteSize = Engine::SpriteSize;

	// constructors
	Game();
	Game(Fleet& f_in);
	~Game();

	// functions
	void gameIdle(Engine& engine, Engine::PlayerInput& keys);

	void getTime(Engine& engine);

	void checkHit(Alien& a, Ship& s);

	void checkHit(Projectile& p, Alien& a);

	void checkHit(Projectile& p, Ship& s);

	void UpdatePlayer(Engine& engine, Engine::PlayerInput& keys);

	void UpdateAliens(Engine& engine);

	void Dashboard(Engine& engine);

	void clearInactive();

	void isPlayerAlive();

	void hasPlayerWon();

	void GameOverMsg(Engine& engine);

	void statePanel();

};
