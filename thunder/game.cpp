#include "game.h"
#include "mapsfiles.h"
#include "utils.h"
#include "GameSleep.h"
#include "GamePrint.h"

#include <stdlib.h>
#include <iostream> 
#include <Windows.h>  

// handles files accoording to the mode given 
void Game::setMode(GameMode _mode, StepInput* _stepsInput, StepsIO* _stepsOutPut) {
	mode = _mode;
	stepInput = _stepsInput;
	switch (mode) {
	case GameMode::SIMPLE:
		break;
	case GameMode::SAVE_TO_FILE:
		resultIO.setMode(ResultIO::FileMode::write);
		stepsOutPut = _stepsOutPut;
		stepsOutPut->setMode(StepsIO::FileMode::write);
		break;
	case GameMode::LOAD_FROM_FILE:
		break;
	case GameMode::SILENT_LOAD_FROM_FILE:
		resultIO.setMode(ResultIO::FileMode::read);
		break;
	default:
		throw std::exception("Game mode is invalid");
	}
}


void Game::prepareToStart()
{
	if (maps.loadMapLevels(levels)) 
	{
		resetBoard();
		health = board.getHealth();
	}
	else 
	{
		throw std::exception("Could not load game maps from mapsfiles, check that you placed the maps in mapfiles directory");
	}
}

void Game::resetBoard()
{
	board.resetBoard();
	if (!maps.getCurrLevelLoadedStatus())
	{
		maps.getMap(board.getOriginalBoard(), mapChoose);
		maps.getlevel(level);
	}	
	board.init(colorSet);
	ships = board.getShips();
	blocks = board.getBlocks();
	time = board.getTime();
	time.setTimeSettings(gameTime, colorSet);
	
}
// In charge of the game finish, if the player finished the level or the entire game
void Game::gameFinish()
{
	clrscr();
	if(level < levels && !mapChoose)
	{
		maps.loadNextMap();
		level++;
		resetBoard();
		health.printHealth();
		freezeShips = true;
		activeShip = GameConfig::BIG_SHIP_ID;
		keyPressed = 0;
	}
	else {
		GamePrint::print("*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*  YOU WON!!!!!  *-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
		printCredits();
	}

}

void Game::printCredits()
{
	GamePrint::print("Finished game, Thank you for playing :D");
	GamePrint::print("*-*-*-*-*-*-*-* Ofri & Or *-*-*-*-*-*-*-*\n");
}
// In charge of the ship actions, if the player finished the level or if the player pressed ESC
void Game::ShipAction() 
{
	if(ships[0].getFinishStatus() && ships[1].getFinishStatus()) // if the player sussecfuly finished the level 
	{
		if(level == levels || mapChoose) // if we are at the last level and win or the player choose the map
			gameState = GameState::WIN;
		if (getMode() == GameMode::SAVE_TO_FILE)
			resultIO.writeEvent(time.getTimeLeft(), Events::FINISH_LEVEL);
		else if (getMode() == GameMode::SILENT_LOAD_FROM_FILE) {
			if (!resultIO.cmpEvents(time.getTimeLeft(), Events::FINISH_LEVEL)) {
				gameState = GameState::RESULT_DIFF;
			}
		}
		gameFinish();
	}
	else
	{
		switch (keyPressed)
		{
		case (int)GameConfig::eKeys::ESC:
			clrscr();
			gameState = GameState::PAUSE;
			keyPressed = 0;
			break;

		case (int)GameConfig::eKeys::SWITCH_TO_BIG_S:
			if (activeShip == GameConfig::ShipID::SMALL)
				activeShip = GameConfig::ShipID::BIG;
			else
				freezeShips = true;
			break;

		case (int)GameConfig::eKeys::SWITCH_TO_SMALL_S:
			if (activeShip == GameConfig::ShipID::BIG)
				activeShip = GameConfig::ShipID::SMALL;
			else
				freezeShips = true;
			break;

		case (int)GameConfig::eKeys::UP:
		case (int)GameConfig::eKeys::DOWN:
		case (int)GameConfig::eKeys::LEFT:
		case (int)GameConfig::eKeys::RIGHT:
			freezeShips = false;
			break;
		};
	}
}

void Game::play() {
	for (auto& pair : *blocks)
		pair.second.move();
	if(not freezeShips && !ships[activeShip].isOverLoaded())
		ships[activeShip].move((GameConfig::eKeys)keyPressed);
}

// In charge of after Death actions, if its by time or by overload
void Game::afterDeath() 
{
	if (getMode() == GameMode::SAVE_TO_FILE) 
	{
		resultIO.writeEvent(time.getTimeLeft(), Events::DEATH);
		stepsOutPut->writeStep(0, 0);
	}
	else if (getMode() == GameMode::SILENT_LOAD_FROM_FILE) 
	{
		if (!resultIO.cmpEvents(time.getTimeLeft(), Events::DEATH)) 
		{
			gameState = GameState::RESULT_DIFF;
		}
	}
	if (health.getlivesLeft() > 1)
	{
		 clrscr();
		 GamePrint::moveToMiddle();
		 if(timeOver)
		 {
			GamePrint::print("!-!-!-!-!-!-!-! TIME OVER !-!-!-!-!-!-!-!");
			timeOver = false;
		 }
		 else
		 {
			 GamePrint::print("!-!-!-!-!-!-!-! SHIP IS OVERLOADED !-!-!-!-!-!-!-!");
		 }
		 GamePrint::moveToMiddle();
		 GamePrint::print("~~~~~~~~~~~~~~~~ Reloading Level ~~~~~~~~~~~~~~~~~~~");
		 health.decreaseLife();
		 GameSleep::longSleep();
		 clrscr();
		 resetBoard();
		 health.printHealth();
		 freezeShips = true;
		 keyPressed = 0;
		 activeShip = GameConfig::BIG_SHIP_ID;
	}
	else
	{
		clrscr();
		GamePrint::print("!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-! GAME OVER !-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!");
		printCredits();
		gameState = GameState::LOSE;
	}
}


/**
 * Runs the game loop, handling player input.
 */
void Game::gameLoop()
{
	if(!pressedPausedInLoadMode)
		keyPressed = 0;
	pressedPausedInLoadMode = false;
	while (gameState==GameState::RUNNING and !timeOver and health.isAlive())
	{
		try {
			if (stepInput->hasInput()) {
				setKey(stepInput->getAction());
				if (mode == GameMode::SAVE_TO_FILE) {
					stepsOutPut->writeStep(keyPressed, time.getTimeLeft());
				}
			}
			ShipAction();
			if (gameState == GameState::RUNNING)
			{
				play();
				timeOver = time.checkAndUpdateTime();
				health.printHealth();
			}

			GameSleep::gameOprSleep();
			if (timeOver || ships[GameConfig::BIG_SHIP_ID].isOverLoaded() || ships[GameConfig::SMALL_SHIP_ID].isOverLoaded())
				afterDeath();
		}
		catch(exception& e) // if the user pressed ESC during load mode
		{
			time.reverse();
			clrscr();
			gameState = GameState::PAUSE;
			pressedPausedInLoadMode = true;
		}

	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameConfig::WHITE);
}