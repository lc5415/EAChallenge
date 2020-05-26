#include "iostream"
#include "Engine.h"
#include "entities.h"
#include <vector>
#include <algorithm>


struct Fleet {
	/*
	Fleet of aliens

	width: number of aliens per row
	height: number of aliens per column
	count: number of aliens in the fleet (at start: count = width*height)
	units: vector of Aliens
	flow_right: whether the fleet is moving towards the right or the left
	
	Down(): all aliens in the units vector take one step down
	*/


	// attributes
	int width, height, count;

	std::vector<Alien> units;
	
	bool flow_right = true;

	// functions
	void Down();

	// {Con/De}structors
	Fleet();
	Fleet(int& w_in, int& h_in);


} ;

class Game {
	/*
	This contains all the game's entities and also control the game's logic:
	Note: I refer to ship as player and to the human player as user

	ATTRIBUTES:
	
	round: round number
	score: game score
	start: whether the game has started (game start when user presses SPACE)

	time_at_lastshot: last recorded time the player shot a rocket
	time_at_lastbomb: last recorded time one of the aliens shot a bomb
	time_at_lastupdate: last recorded time the game state was updated
	delay_btwn_shots: minimum delay between player rocket firing
	delay_btwn_bombs: minimum delay between alien's bombing
	delay_btwn_updates: minimum delay between game state updates (so that it runs
	 at the same rate across platforms - regardless of CPU clock)

	l_msg, s_msg, r_msg: default string to be printed in the dashboard
	idleMsg: message to be displayed while game is idle
	score_msg: string with the game score as a string (updates when score updates)
	round_msg: string with the game round as a string
	whatever_msg_x, whatever_msg_y: x and y position of each message 

	player: game's player (aka ship)
	aliens: fleet of aliens (struct)
	rockets: vector of rockets
	bombs: vector of bombs

	FUNCTIONS:

	void gameIdle(Engine& engine, Engine::PlayerInput& keys);
		displayes message while game has not started (until user presses key)

	void checkHit(Alien& a, Ship& s);
		check hits between any alien and the ship, if hit occurs, alien dies and
		ship loses one life.

	void checkHit(Projectile& p, Alien& a);
		check hits between projectiles and alien, if a hit occurs both alien
		and projectile(rocket) dissapear (become inactive)

	void checkHit(Projectile& p, Ship& s);
		check hits between projectile and ship, if hit occurs the projectile 
		dissapears and the ship loses one life

	void Draw(Engine& engine);
		draw all active entities and dashboard

	void UpdatePlayer(Engine& engine, Engine::PlayerInput& keys);
		Update the player (ship), manages the movement and shooting of the
		ship

	void UpdateAliens(Engine& engine);
		update aliens positions and manages bomb shooting

	void Dashboard(Engine& engine);
		displays dashboard with # of lifes, score and round number

	void clearInactive();
		Goes through aliens.units, rockets and bombs vectors and clears
		inactive (active = false) entries from memory

	void isPlayerAlive();
		check if player has any lives left and if it does not, sets player active
		status to false

	void hasPlayerWon();
		check if there are any aliens left, if there are a fresh fleet is spawned
		with the same characteristics as the previous

	void GameOverMsg(Engine& engine);
		displays final score and game over msg

	void statePanel();
		prints size of: rockets vector, bombs vector, aliens.units vector and score
		in the command line. Can be used to monitor size of the vectors.
		Not called in EngineMain() by default

	*/
public:
	// game attributes
	int round = 1;
	int score = 0; 
	bool start = false;

	// time management attributes
	double time_at_lastshot;
	double time_at_lastbomb;
	double time_at_lastupdate;
	double delay_btwn_shots = 0.5; //half a second
	double delay_btwn_bombs = 0.25; // make aliens shoot fast as twice as ship
	double delay_btwn_updates = 0.01;

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

	// constructors
	Game();
	Game(Fleet& f_in, double& masterDelay);
	~Game();

	// functions
	void gameIdle(Engine& engine, Engine::PlayerInput& keys);

	void getTime(Engine& engine);

	void checkHit(Alien& a, Ship& s);

	void checkHit(Projectile& p, Alien& a);

	void checkHit(Projectile& p, Ship& s);

	void Draw(Engine& engine);

	void UpdatePlayer(Engine& engine, Engine::PlayerInput& keys);

	void UpdateAliens(Engine& engine);

	void Dashboard(Engine& engine);

	void clearInactive();

	void isPlayerAlive();

	void hasPlayerWon();

	void GameOverMsg(Engine& engine);

	void statePanel();

};
