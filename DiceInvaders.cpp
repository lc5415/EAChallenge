#include "Game.h"

/////////MAIN BODY///////////
void EngineMain()
{	

	int w_aliens = 8; // width of alien fleet
	int h_aliens = 4; // height of alien fleet

	Engine engine; // initalise engine
	Engine::PlayerInput keys; // initialise key input

	Fleet aliens(w_aliens, h_aliens);// initialise fleet of aliens
	double delay_btwn_updates = 0.015; // time it will take for dynamics to change
	
	Game game(aliens, delay_btwn_updates); // initialise game
	// initialise player's x position
	game.player.x = (engine.CanvasWidth-engine.SpriteSize) / 2;

	// initialise times_at_last"event"
	game.time_at_lastshot = engine.getStopwatchElapsedSeconds();
	game.time_at_lastbomb = engine.getStopwatchElapsedSeconds();
	game.time_at_lastupdate= engine.getStopwatchElapsedSeconds();

	while (engine.startFrame())
	{	

		/*
		Get player input as this is fundamental before the game 
		starts and during the game
		*/
		keys = engine.getPlayerInput();

		if (! game.start){
			/* 
			INITIAL WINDOW:
			Wait until user presses space to start the game 
			*/ 
			game.gameIdle(engine, keys); // display PRESS SPACE TO START until space is pressed

		} else { // start game

			game.Draw(engine); // Draw all entities

				if (game.player.active) // if the player is still alive
				{	
					// only update if time between updates has passed
					if (engine.getStopwatchElapsedSeconds()-game.time_at_lastupdate>game.delay_btwn_updates)
					{	
						// update time_at_lastupdate
						game.time_at_lastupdate = engine.getStopwatchElapsedSeconds();

						game.UpdatePlayer(engine, keys); // update position and shoot

						game.UpdateAliens(engine); // update position and bomb
						
						game.clearInactive(); // clear out memory of no longer active entities

						game.isPlayerAlive(); // check whether player is still alive

						game.hasPlayerWon(); // check whether player has won (no more aliens) and
						// if so respawn a new fleet of aliens
					}

				} else { //if player not active
					game.GameOverMsg(engine); // display game over message with final score
				}

			}
		}


	}


