#ifndef BLOCK_H
#define BLOCK_H

#include "point.h"
#include "utils.h"

class Board;

class Block
{
private:
	static GameConfig::Color backgroundcolor;

	Board* board;
	Point pos[GameConfig::MAX_BLOCK_SIZE];
	Point nextPos[GameConfig::MAX_BLOCK_SIZE];
	size_t size = 0;
	LocationInfo locationInfo;
	GameConfig::gamePieceType type = GameConfig::SECONDARY;
	char symbol = NULL;

	void delTrace();

public:
	Block(){};
	Block(char symbol, Board* board) : symbol(symbol), board(board) {};

	static GameConfig::Color getBackgroundColor() { return backgroundcolor; }
	static void setColor(GameConfig::Color color) { backgroundcolor = color; }	
	
	void addPoint(int x, int y) { pos[size++].set(x, y); }
	bool move(GameConfig::eKeys direction = GameConfig::eKeys::DOWN, int* carryWeight = nullptr, bool onCommand = false);
	GameConfig::gamePieceType getType() { return type; }
	char getSymbol() { return symbol; }
	bool checkMove(GameConfig::eKeys direction, int* carryWeight);
	size_t getSize() const { return size; }
	Point getPos(size_t i) const { return pos[i]; }
	bool static isBlock(char ch);
	
};

#endif
