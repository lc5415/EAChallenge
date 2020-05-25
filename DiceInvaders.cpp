#include "Game.h"

/////////MAIN BODY///////////
void EngineMain()
{	

	int w_aliens = 8; // width of alien fleet
	int h_aliens = 4; // height of alien fleet
	Engine engine;
	Engine::PlayerInput keys;
	Fleet aliens(w_aliens, h_aliens);
	Game game(aliens);
	game.player.x = (engine.CanvasWidth-engine.SpriteSize) / 2;

	game.time_at_lastshot = engine.getStopwatchElapsedSeconds();
	game.time_at_lastbomb = engine.getStopwatchElapsedSeconds();
	game.time_at_laststep = engine.getStopwatchElapsedSeconds();

	while (engine.startFrame())
	{	

		/*
		Get player input as this is fundamental before the game 
		starts and during the game
		*/
		
		keys = engine.getPlayerInput(); // get keyboard state

		/* INITIAL WINDOW:
		Wait until user presses space to start the game */ 
		if (! game.start){
			
			game.gameIdle(engine, keys); // display PRESS SPACE TO START until space is pressed

		} else { // start game

			game.getTime(engine); // get the current time (game time)
			game.Draw(engine); // draw at every timestep
					// Move aliens if player is active
				if (game.player.active) // if the player is still alive
				{	
					if (game.timestamp-game.time_at_laststep>game.delay_btwn_steps) // update "masterclock")
					{	
						game.time_at_laststep = game.timestamp;
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


