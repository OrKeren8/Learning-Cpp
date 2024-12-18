#pragma once

#include "point.h"
#include "ship.h"
#include "block.h"
#include "gameConfig.h"
#include "time.h"
#include "health.h"
#include "utils.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <map>


class Board {
	// Board Details 
	constexpr static size_t WIDTH = GameConfig::GAME_WIDTH;
	constexpr static size_t HEIGHT = GameConfig::GAME_HEIGHT;
	bool colorSet = false;
	char original_board[HEIGHT][WIDTH + 1];
	bool mapFileLoaded = false;

	// Board Pieces
	Time time;
	Health health;
	Point life_pos;
	Point exit_pos;	
	Ship ships[GameConfig::NUM_SHIPS];
	map<char, Block> blocks;
	size_t num_blocks = 0;


	Ship* getShipBySymbol(char sym);

public:
	char board[HEIGHT][WIDTH + 1];  // the actual board that will be modified
	
	// getters
	Ship* getShips()  {return ships;}
	map <char, Block> * getBlocks() { return &blocks; }
	Time& getTime()  { return time; }
	Health& getHealth()  { return health; }
	char(&getOriginalBoard())[HEIGHT][WIDTH + 1]{return original_board;}

    // modifiers and checkers
 	void init(bool colorSet);
	void printScreen();
	void updateGamePieces();
	bool checkMove(LocationInfo& objectLoction);
	bool checkFall(LocationInfo& objectLoction,Block* cargoBlock = nullptr, char keyCargoBlock = '\0');
	void shipFinishLine(char shipID);
	void resetBoard();
	bool checkBlockCrash(LocationInfo& ol, bool& stillCarried, map <char, Block*>& obsticals);
	
};