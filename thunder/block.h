#ifndef BLOCK_H
#define BLOCK_H

#include "point.h"
#include "utils.h"

class Board;
class Ship;
class Block
{
private:
	Board* board;
	void delTrace();
	
	// Block Details
	static GameConfig::Color backgroundcolor;
	unsigned int size = 0;
	LocationInfo locationInfo;
	Point pos[GameConfig::MAX_BLOCK_SIZE];
	Point nextPos[GameConfig::MAX_BLOCK_SIZE];
	char symbol;
	Ship* carrierShip = nullptr;


public:
	Block(){};
	Block(char symbol, Board* board) : symbol(symbol), board(board) {};
	
	// getters
	unsigned int getSize() const { return size; }
	Point getPos(size_t i) const { return pos[i]; }
	static GameConfig::Color getBackgroundColor() { return backgroundcolor; }
	bool static isBlock(char ch);
	char getCarrierShipID() const;
	bool isCarriedBlock() const { return carrierShip != nullptr; }
	Ship* getCarrierShip() const { return carrierShip; }
	
	// setters
	static void setColor(GameConfig::Color color) { backgroundcolor = color; }	
	char getSymbol() const { return symbol; }
	void setCarrierShip(Ship* carrierShip) { this->carrierShip = carrierShip; }
	void removeCarrierShip() { carrierShip = nullptr; }
	
	// modifiers and checkers
	void addPoint(int x, int y) { pos[size++].set(x, y); }
	bool move(GameConfig::eKeys direction = GameConfig::eKeys::DOWN, int* carryWeight = nullptr, bool onCommand = false);
	bool checkMove(GameConfig::eKeys direction, int* carryWeight);
	bool checkFall(Block* blockToCarry = nullptr, char keyCargoBlock = '\0');
};

#endif
