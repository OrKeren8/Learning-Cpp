#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include  <stdbool.h>
#include <map>

#include "ship.h"
#include "board.h"
#include "utils.h"
#include "time.h"
#include "gameConfig.h"
#include "stepsIO.h"
#include "StepInput.h"
#include "KeyboardInput.h"
#include "ResultIO.h"
#include "mapsfiles.h"

using namespace std;

class Ship;

enum class GameState {
	RUNNING = 0, //The game shall run 
	PAUSE, //puase the game
	WIN, //the game is finished and the player won
	LOSE, //the game is finished and the player lost
	RESULT_DIFF //there is a difference between the curr result and the recorded one
};

enum class GameMode {
	SIMPLE = 0,
	SAVE_TO_FILE,
	LOAD_FROM_FILE,
	SILENT_LOAD_FROM_FILE
};

class Game
{
	//game piece, resultIOs, stepsIOs
	Board board;
	Mapsfiles maps;
	Point timelocation;
	StepInput* stepInput = nullptr;
	StepsIO* stepsOutPut = nullptr;
	ResultIO resultIO;
	Health health;
	Time time;
	Ship* ships;
	map <char, Block>* blocks;

	
	
	//data global indicators and Details
	int activeShip = 0; // 0 - Big Ship, 1 - Small Ship	
	bool freezeShips = false;
	int gameTime = GameConfig::GAME_TIME;
	size_t level = 1;
	size_t levels;
	int keyPressed;
	bool mapfileLoaded;
	bool pressedPausedInLoadMode = false;
	GameState gameState;
	GameMode mode = GameMode::SIMPLE;
	bool timeOver = false;
	bool colorSet = true;
	bool mapChoose = false;

	// private functions of game
	
	// setters
	void setKey(int key) { keyPressed = tolower(key); }
	// modifiers and checkers
	void ShipAction();
	void play();
	void resetBoard();
	void afterDeath();
	void gameFinish();

public:

	Game():resultIO(level) {};
	
	// getters
	GameMode getMode() const { return mode; }
	bool getMapFileStatus() const { return mapfileLoaded; }
	GameState getState() const { return gameState; }
	const size_t& getTimeLeft() { return time.getTimeLeft(); }
	const size_t& getLevel() { return level;}

	// setters
	void setMode(GameMode _mode, StepInput* _stepsInput, StepsIO* _stepsOutPut);
	void setStateToRunning() { gameState = GameState::RUNNING; }
    void setColorSet(bool colorSet) { this->colorSet = colorSet; }
	void setMapChoose(bool mapChoose) { this->mapChoose = mapChoose; }
	void SetfreezeShipsOn() { freezeShips = true; }
	
	// modifiers and checkers
	void prepareToStart();
	void gameLoop();
	void printScreen() { board.printScreen(); }
	void printCredits();

};

#endif