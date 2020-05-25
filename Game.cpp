#include "Game.h"


Fleet::Fleet(){}
Fleet::Fleet(int& w_in, int& h_in):width(w_in), height(h_in){

	// allocate memory 
	count = w_in*h_in;	
	units.resize(count);

	//Give in initial position
	for (int j = 0; j<h_in; j++)
	{
		for (int i = 0; i<w_in;i++)
		{
			units[i+j*w_in].x = Engine::SpriteSize*i;
			units[i+j*w_in].y = Engine::SpriteSize*j;
		}
	}
}

void Fleet::Down(){
	for (Alien& a: units)
		a.y+=Engine::SpriteSize;
};


// Game class


Game::Game(){}

Game::Game(Fleet& f_in)
:aliens(f_in)
{}

Game::~Game(){}


void Game::gameIdle(Engine& engine, Engine::PlayerInput& keys){
	engine.drawText(
		idleMsg, 
		(engine.CanvasWidth - (sizeof(idleMsg) - 1) * engine.FontWidth) / 2, 
		(engine.CanvasHeight - engine.FontRowHeight) / 2); 

	if (keys.fire) start = true; // if hit space start game
};

void Game::getTime(Engine& engine){
	timestamp = engine.getStopwatchElapsedSeconds();
	// (void)timestamp;
	iter++;
};


void Game::checkHit(Projectile& p, Alien& a)
{/*This check hits between rockets and aliens and add score*/
const int size = spriteSize;
if (p.x + size > a.x &&
	a.x+size > p.x && 
	a.y+size > p.y &&
	p.y+size > a.y)
{
	p.active = false;
	a.active = false;
	score++;
	score_msg = std::to_string(score);
}

};

void Game::checkHit(Projectile& p, Ship& s)
{/*This check hits between the bombs and the player and updates the player
life*/
const int size = Engine::SpriteSize;
if (p.x + size > s.x &&
	s.x+size > p.x && 
	s.y+size > p.y &&
	p.y+size > s.y)
{
	s.lives--;
	s.lives_msg = std::to_string(s.lives);
	p.active = false;
}
};

void Game::checkHit(Alien& a, Ship& s)
{/*This check hits between the bombs and the player and updates the player
life*/
if (a.x + spriteSize > s.x &&
	s.x+spriteSize > a.x && 
	s.y+spriteSize > a.y &&
	a.y+spriteSize > s.y)
{
	s.lives--;
	s.lives_msg = std::to_string(s.lives);
	a.active = false;
}
};




void Game::UpdatePlayer(Engine& engine, Engine::PlayerInput& keys){
	// ship movement + display
	if (keys.left && player.x>0)  --player.x;
	if (keys.right &&
		player.x < engine.CanvasWidth-engine.SpriteSize) ++player.x;

		engine.drawSprite(
			player.sprite, 
			player.x, player.y);

	// ship shooting
	if (keys.fire &&
		engine.getStopwatchElapsedSeconds()-time_at_lastshot>delay_btwn_shots)
	{
		rockets.emplace_back(player.x, player.y); 
		// equiv to .push_back(Rocket(x, y_player))
		time_at_lastshot = engine.getStopwatchElapsedSeconds();
	}
	// Rockets display+movement
	for (Rocket& r: rockets) // reference to avoid copying
	{
		if (r.active){
			engine.drawSprite(
				r.sprite, 
				r.x, r.y);
			r.move();
		}
	}

};

void Game::UpdateAliens(Engine& engine)
{
	for (Alien& a: aliens.units)
	{	
		if (a.active)
		{
			if (aliens.flow_right)
			{
				a.x++;
			// Check if unit hit right edge
				if (a.x == engine.CanvasWidth-engine.SpriteSize)
				{
					aliens.flow_right = ! aliens.flow_right;
					aliens.Down();
				}
			} else {
				a.x--;
			// Check if unit hit right edge
				if (a.x == 0)
				{
					aliens.flow_right = ! aliens.flow_right;
					aliens.Down();
				}
			}

		// display
			engine.drawSprite(
				a.sprite, 
				a.x, a.y);

		//bombs
			if (engine.getStopwatchElapsedSeconds()-time_at_lastbomb>delay_btwn_bombs)
			{	
				if (a.Shoot(bombs)) time_at_lastbomb = engine.getStopwatchElapsedSeconds(); 
			}

		//check collisions with rockets
			for (Rocket& r: rockets)
			{	
				if (r.active) checkHit(r, a);
			}

		//check collisions with player
			checkHit(a, player);

		// check if they are at the bottom of the screen
			if (a.y == engine.CanvasHeight) player.active = false;
		}

		aliens.count -= !(a.active); // update count by substracting 
		// no longer active aliens
		
	}


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
};


void Game::Dashboard(Engine& engine){
	//print lives
	engine.drawText((l_msg+player.lives_msg).c_str(),
		l_msg_x, l_msg_y); 
	//print score
	engine.drawText((s_msg+score_msg).c_str(),
		s_msg_x, s_msg_y);
	engine.drawText((r_msg+round_msg).c_str(),
		r_msg_x, r_msg_y);
};

void Game::clearInactive(){
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
	rockets.erase(
		std::remove_if(rockets.begin(), rockets.end(),
			[](Rocket& r){
				bool dissapear = !(r.active) || (r.y<0);
				return dissapear;
			}), rockets.end());
	


	//clear out aliens
	aliens.units.erase(
		std::remove_if(aliens.units.begin(),
			aliens.units.end(),
			[](Alien& a){

				bool dissapear = !(a.active) or
				(a.y>Engine::CanvasHeight);
				return dissapear;

			}), aliens.units.end()); 
	


	//clear out bombs
	bombs.erase(
		std::remove_if(bombs.begin(),
			bombs.end(),
			[](Bomb& b){

				bool dissapear = !(b.active) or
				(b.y > Engine::CanvasHeight-Engine::SpriteSize);
				return dissapear;

			}), bombs.end()); 
	

};

void Game::isPlayerAlive(){
	if (player.lives==0) player.active = false;
};

void Game::hasPlayerWon(){
	if (aliens.count <1){ // if less than one player alive, respawn aliens
		aliens = Fleet(aliens.width, aliens.height);
		round++;
		round_msg = std::to_string(round);
	}
}

void Game::GameOverMsg(Engine& engine){


	// FREEZE SPRITE AT DEFEAT

	for (Alien& a: aliens.units){
	engine.drawSprite(
		a.sprite,a.x, a.y);
	}

	for (Bomb&b: bombs){
			engine.drawSprite(
			b.sprite, 
			b.x, b.y);
	}


	engine.drawSprite(player.sprite, player.x, player.y);


	// GAME OVER MESSAGE
	std::string final_msg = "FINAL SCORE: " + score_msg;
	engine.drawText(
		final_msg.c_str(), 
		(engine.CanvasWidth - (sizeof(final_msg) - 1) * engine.FontWidth) / 2, 
		((engine.CanvasHeight - engine.FontRowHeight) / 2)-engine.SpriteSize);
	const char message[] = "GOOD LUCK NEXT TIME. Press Esc to leave";
	engine.drawText(
		message, 
		(engine.CanvasWidth - (sizeof(message) - 1) * engine.FontWidth) / 2, 
		((engine.CanvasHeight - engine.FontRowHeight) / 2)+engine.SpriteSize); 
};

void Game::statePanel(){
	printf("\rRocket size: %lu, Bombs: %lu, Aliens: %lu, Score: %d",
		rockets.size(),  bombs.size(), aliens.units.size(), score);

}





















