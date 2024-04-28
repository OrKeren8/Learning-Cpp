#pragma once

#include "point.h"
#include "ship.h"
#include "block.h"
#include "gameConfig.h"

#include <iostream>
#include <cstring>

class Board {
	constexpr static size_t WIDTH = 80;
	constexpr static size_t HEIGHT = 25;
	constexpr static size_t NUM_SHIPS = GameConfig::NUM_SHIPS;
	constexpr static size_t NUM_BLOCKS = 1;
	// the original board that will be copied to the actual board
	char original_board[HEIGHT][WIDTH + 1] = {
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
			"W                                                                              W", // 1
			"W        &                                                                     W", // 2
			"W                                                                              W", // 3
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 4
			"W         @@                                                                   W", // 5
			"W         ##                                                                   W", // 6
			"W         ##                                                                   W", // 7
			"W                                                                              W", // 8
			"W                                                                              W", // 9
			"W                                                                              W", // 10
			"W                                                                              W", // 11
			"W                                                                              W", // 12
			"W                                                 WWWWWWWWWWWW                 W", // 13
			"W                                                                              W", // 14
			"W                                                 W         WWWW               W", // 15
			"W                                             WWWWW            W               W", // 16
			"W                                              00              W               W", // 17
			"W                                       W      00            X W               W", // 18
			"W                                       WWWWWWWWWWWWWWWWWWWWWWWW               W", // 19
			"W                                                                              W", // 20
			"W                                                                              W", // 21
			"W                                                                              W", // 22
			"W                                                                              W", // 23
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	};
	Block blocks[NUM_BLOCKS];
	Point legend_pos;
	Point exit_pos;	
	Ship ships[NUM_SHIPS];

public:
	// the actual board that will be modified
	char board[HEIGHT][WIDTH + 1];
	void init(bool colorSet);
	void printScreen();
	void updateGamePieces();
	bool checkCollision(Point * nextPos,size_t size, char idSymbol);
	void getShips(Ship ships[]) const { memcpy(ships, this->ships, sizeof(this->ships)); }
	void getBlocks(Block blocks[]) const { memcpy(blocks, this->blocks, sizeof(this->blocks)); }
};

// this is a test