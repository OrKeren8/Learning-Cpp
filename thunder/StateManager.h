#pragma once
#include "game.h"

class StateManager
{
	// StateManager Details
	bool mapChoose = false;
	bool colorSet = true;
	static bool toExit;
	
	// stateManger objects
	Game game;
	StepsIO stepsIO;
	KeyboardInput keyboardStepsInput;

	// private functions (helper functions)
	void mainMenu();
	void pauseMenu();
	void setMode(int argc, char* argv[]);


	
public:
	StateManager(int argc, char* argv[]);
	static void exceptionHandler(const exception& e);
	void startGame();
};

