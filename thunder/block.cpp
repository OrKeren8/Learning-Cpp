#include "ship.h"
#include "board.h"
#include "gameConfig.h"

#include "utils.h"
#include <cstring>


GameConfig::Color Block::backgroundcolor = GameConfig::BLOCK_COLOR;


bool Block::isBlock(char ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		return true;
	}
	return false;
}


bool Block::move(GameConfig::eKeys direction, int* carryWeight, bool onCommand)
{		
	if (!onCommand) // if we on command to move we dont need to check anything 
	{
		if (direction != GameConfig::eKeys::DOWN) 
		{
			if (!checkMove(direction, carryWeight)) 
			{
				return false;
			}
		}
		else // direction is DOWN
		{
			if (!checkFall()) 
			{
				return false;
			}
		}
	}
		delTrace();
		std::copy(std::begin(nextPos), std::end(nextPos), std::begin(pos));
		int currY, currX;
		for (size_t i = 0; i < size; i++)
		{
			currY = pos[i].getY();
			currX = pos[i].getX();
			pos[i].draw(symbol, backgroundcolor);
			board->board[currY][currX] = symbol;
		}
		hideCursor();
		return true;	
}
	


bool Block::checkMove(GameConfig::eKeys direction, int* carryWeight) {
	bool canMove = false;
	if (!(carryWeight != nullptr && ((*carryWeight -= (int)this->size) < 0))){//if there is carryWeight and it is negative return false
		std::copy(std::begin(pos), std::end(pos), std::begin(nextPos));
		for (size_t i = 0; i < size; i++)
			nextPos[i].move(direction);
		locationInfo = { nextPos, symbol, size, direction, carryWeight };
		if (board->checkMove(locationInfo))
			canMove = true;
	}
	return canMove;
}

bool Block::checkFall(Block* blockToCarry, char keyCargoBlock)
{
	bool canMove = false;
	std::copy(std::begin(pos), std::end(pos), std::begin(nextPos));
	for (size_t i = 0; i < size; i++)
		nextPos[i].move(GameConfig::eKeys::DOWN);
	locationInfo = { nextPos, symbol, size, GameConfig::eKeys::DOWN};
	if (board->checkFall(locationInfo,blockToCarry,keyCargoBlock))
		canMove = true;
	return canMove;
}

void Block::delTrace() {
	int currY, currX;
	for (unsigned int i = 0; i < size; i++)
	{
		currY = pos[i].getY();
		currX = pos[i].getX();
		pos[i].draw(' ', GameConfig::BLACK);
		board->board[currY][currX] = ' ';
	}
}
char Block::getCarrierShipID() const
{
	if (carrierShip != nullptr)
		return carrierShip->getSymbol();
	return '\0';
}