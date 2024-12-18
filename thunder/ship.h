#ifndef SHIP_H
#define SHIP_H
#include <map>
#include "point.h"
#include "utils.h"
class Board;
class Block;
using std::map;
class Ship
{
private:
	// Ship Details
	Point pos[GameConfig::MAX_SHIP_SIZE];
	Point nextPos[GameConfig::MAX_SHIP_SIZE];
	Point finishPos[GameConfig::MAX_SHIP_SIZE];
	size_t size = 0;
	size_t finishSize = 0;
	char symbol = '\0';
	bool isFinished = false;
	unsigned int maxCarryWeight;
	unsigned int trunkWeight = 0;
	GameConfig::Color backgroundcolor = GameConfig::WHITE;
	LocationInfo shipLocationinfo;
	map<const char,Block*> trunk;
	
	Board* board;
	
	// private ship functions (helpers)
	void delTrace();
	LocationInfo& checkNextObjLocation(GameConfig::eKeys direction, int* carryWeight);
	void removeAllBlocksFromTrunk();
public:
	// getters
	void init(char symbol, int maxCarryWeight, GameConfig::Color color, Board* Board);
	size_t getSize() const { return size; }
	Point getPos(size_t i) const { return pos[i]; }
	char getSymbol() const { return symbol; }
	bool getFinishStatus() const { return isFinished; }
	int getMaxCarryWeight() const { return maxCarryWeight; }
	
	// modifiers and checkers
	GameConfig::Color getBackgroundColor() const { return backgroundcolor; }
	void addPoint(int x, int y) { pos[size++].set(x, y); }
	void addFinishPoint(int x, int y) { finishPos[finishSize++].set(x, y); }
	bool move(GameConfig::eKeys direction);
	void shipFinishLine();
	static bool isShip(char ch);
	void addToTrunk(const char key, Block* block);
	void removeFromTrunk(const char key, Block& block);
	bool isOverLoaded() const { return trunkWeight > maxCarryWeight; }
};

#endif
