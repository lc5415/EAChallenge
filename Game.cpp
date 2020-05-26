#include "Game.h"


Fleet::Fleet(){}
Fleet::Fleet(int& w_in, int& h_in):width(w_in), height(h_in){
	/*
	Makes 1D vector instead of 2D for more efficient memory access
	*/

	// set count and allocate memory to empty vector
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
	/* All units take one step down, step size is the aliens height*/
	for (Alien& a: units)
		a.y+=a.height;
};


// Game class


Game::Game(){}

Game::Game(Fleet& f_in, double& masterDelay)
:aliens(f_in), delay_btwn_updates(masterDelay)
{	
	/*
	This step ensures that whatever the speed that the game runs at
	the rate of firing of the units remains the same (with respect
	to the game speed).

	These numbers are sort of arbitrary. The right choice is
	eventually chosen by the user. Could be calibrated with 
	a pre-game calibration function 
	*/

	delay_btwn_shots = 50*delay_btwn_updates;
	delay_btwn_bombs = 25*delay_btwn_updates;
}

Game::~Game(){}


void Game::gameIdle(Engine& engine, Engine::PlayerInput& keys){
	/*
	Draws idleMsg until SPACE is pressed (and start changes to true)
	*/
	engine.drawText(
		idleMsg, 
		(engine.CanvasWidth - (sizeof(idleMsg) - 1) * engine.FontWidth) / 2, 
		(engine.CanvasHeight - engine.FontRowHeight) / 2); 

	if (keys.fire) start = true; // if hit space start game
};


void Game::checkHit(Projectile& p, Alien& a)
{
/*This check hits between rockets and aliens and add score*/

	if (p.x + p.width > a.x && // overlap conditions
		a.x+a.width > p.x && 
		a.y+a.height > p.y &&
		p.y+p.height > a.y)
	{	
		// if hit occurs set entities to be inactive
		p.active = false;	
		a.active = false;
		//update score
		score++;
		score_msg = std::to_string(score);
	}

};

void Game::checkHit(Projectile& p, Ship& s)
{
/*This check hits between the bombs and the player and updates the player
life*/
	if (p.x + p.width> s.x && // same overlap conditions as before
		s.x+s.width > p.x && 
		s.y+s.height > p.y &&
		p.y+p.height > s.y)
	{
		// if hit occurs decrease ship's life count and update lives string
		s.lives--;
		s.lives_msg = std::to_string(s.lives);
		// set projectile to inactive
		p.active = false;
	}
};

void Game::checkHit(Alien& a, Ship& s)
{
/*This check hits between the bombs and the player and updates the player
life*/
	if (a.x + a.width > s.x && // same overlap conditions as before
		s.x+s.width > a.x && 
		s.y+s.height > a.y &&
		a.y+a.height > s.y)
	{	
		// if hit occurs decrease ship's life count and update lives string
		s.lives--;
		s.lives_msg = std::to_string(s.lives);
		// set alien to inactive
		a.active = false;
	}
};




void Game::UpdatePlayer(Engine& engine, Engine::PlayerInput& keys){
	
	// if left and player is within bounds
	if (keys.left && player.x>0)  --player.x;
	
	// if right and player is within bounds
	if (keys.right &&
		player.x < engine.CanvasWidth-engine.SpriteSize) ++player.x;

	// ship shooting: if SPACE is pressed and the delay between shots has
	// passed
	if (keys.fire &&
		engine.getStopwatchElapsedSeconds()-time_at_lastshot>delay_btwn_shots)
	{
		player.Shoot(rockets);
		time_at_lastshot = engine.getStopwatchElapsedSeconds(); //update time of last shot
	}

	for (Rocket& r: rockets)
	{
		r.move(); // move all rockets
	}

};

void Game::UpdateAliens(Engine& engine)
{
	for (Alien& a: aliens.units)
	{	

		// if the aliens are moving to the right, keep increasing
		// x; if any aliens hits the bounds all aliens go down and
		// the flow direction is inverted.
		if (aliens.flow_right)
		{
			a.x++;
			// Check if unit hit right edge
			if (a.x == engine.CanvasWidth-a.width)
			{
				aliens.flow_right = ! aliens.flow_right;
				aliens.Down();
			}
		} else {
			a.x--;
			// Check if unit hit left edge
			if (a.x == 0)
			{
				aliens.flow_right = ! aliens.flow_right;
				aliens.Down();
			}
		}

		//bombs
		if (engine.getStopwatchElapsedSeconds()-time_at_lastbomb>delay_btwn_bombs)
		{	
			if (a.Shoot(bombs)) time_at_lastbomb = engine.getStopwatchElapsedSeconds(); 
		}

		//check collisions with rockets
		for (Rocket& r: rockets)
		{	
			checkHit(r, a);
		}

		//check collisions with player
		checkHit(a, player);

		// check if they are at the bottom of the screen:
		// I understood bottom as pass the line where the ship
		// sits but I don't think that is ever achievable
		if (a.y == engine.CanvasHeight) player.active = false; 
		

		aliens.count -= !(a.active); // update count by substracting 
		// no longer active aliens
		
	}


	// bombs display + movement
	for (Bomb& b: bombs) // reference to avoid copying
	{	
		b.move();

		//check for collisions with the player
		checkHit(b, player);
	}
};

void Game::Draw(Engine& engine){
	

	// Draw aliens
	for (Alien& a: aliens.units){
		engine.drawSprite(
			a.sprite,a.x, a.y);
	}

	// Draw bombs
	for (Bomb& b: bombs){
		engine.drawSprite(
			b.sprite, 
			b.x, b.y);
	}

	// Draw rockets
	for (Rocket& r: rockets)
	{
		engine.drawSprite(
			r.sprite, 
			r.x, r.y);

	}

	// Draw player
	engine.drawSprite(player.sprite, player.x, player.y);

	// display dashboard
	Dashboard(engine);

}


void Game::Dashboard(Engine& engine){
	
	//print lives
	engine.drawText((l_msg+player.lives_msg).c_str(),
		l_msg_x, l_msg_y); 

	//print score
	engine.drawText((s_msg+score_msg).c_str(),
		s_msg_x, s_msg_y);

	//print round
	engine.drawText((r_msg+round_msg).c_str(),
		r_msg_x, r_msg_y);
};

void Game::clearInactive(){
	/*
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
				// conditions for dissapearance
				bool dissapear = !(r.active) || (r.y<0);
				return dissapear;
			}), rockets.end());
	


	//clear out aliens
	aliens.units.erase(
		std::remove_if(aliens.units.begin(),
			aliens.units.end(),
			[](Alien& a){
				// conditions for dissapearance
				bool dissapear = !(a.active) || 
				(a.y>Engine::CanvasHeight);
				return dissapear;
			}), aliens.units.end()); 
	


	//clear out bombs
	bombs.erase(
		std::remove_if(bombs.begin(),
			bombs.end(),
			[](Bomb& b){
				// conditions for dissapearance
				bool dissapear = !(b.active) ||
				(b.y > Engine::CanvasHeight-Engine::SpriteSize);
				return dissapear;
			}), bombs.end()); 
	

};

void Game::isPlayerAlive(){
	if (player.lives==0) player.active = false;
};

void Game::hasPlayerWon(){
	if (aliens.count <1){ 
	// if less than one player alive, respawn aliens and
		// updates round numbers
		aliens = Fleet(aliens.width, aliens.height);
		round++;
		round_msg = std::to_string(round);
	}
}

void Game::GameOverMsg(Engine& engine){


	// FREEZE SPRITE AT DEFEAT
	Draw(engine);

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





















