#include "block.h"
#include "utils.h"
#include "board.h"


void Ship::init(char symbol, int maxCarryWeight, GameConfig::Color color, Board *board)
{
	this->symbol = symbol;
	this->maxCarryWeight = maxCarryWeight;
	this->backgroundcolor = color;
	this->board = board;
}


bool Ship::isShip(char ch)
{
	if (ch == GameConfig::BIG_SHIP_S || ch == GameConfig::SMALL_SHIP_S)
	{
		return true;
	}
	return false;
}


bool Ship::move(GameConfig::eKeys direction)
{
	if (!isFinished) {
		int carryWeight = maxCarryWeight;
		bool carriedBlocksCanMove = true;
		Block* currBlock = nullptr;
		if(direction != GameConfig::eKeys::DOWN){
			for (auto& block : trunk)
			{
				currBlock = block.second;
				int blockCarryWeight = currBlock->getSize();
				if (!(currBlock->move(direction, &blockCarryWeight)))
				{
					carriedBlocksCanMove = false;
				}
			}
		}
		if (board->checkMove(checkNextObjLocation(direction, &carryWeight)) && carriedBlocksCanMove) {
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
	}
	return false;
}


LocationInfo& Ship::checkNextObjLocation(GameConfig::eKeys direction, int* carryWeight) {
	std::copy(std::begin(pos), std::end(pos), std::begin(nextPos));
	for (size_t i = 0; i < size; i++)
		nextPos[i].move(direction);
	shipLocationinfo = { nextPos, symbol, size , direction, carryWeight};
	return shipLocationinfo;
}


void Ship::delTrace() {
	int currY, currX;
	for (int i = 0; i < size; i++) 
	{
		currY = pos[i].getY();
		currX = pos[i].getX();
		pos[i].draw(' ', GameConfig::BLACK);
		board->board[currY][currX] = ' ';
	}
}

void Ship::shipFinishLine()
{
	isFinished = true;
	delTrace();
	std::copy(std::begin(finishPos), std::end(finishPos), std::begin(pos));
	int currY, currX;
	for (size_t i = 0; i < size; i++)
	{
		currY = pos[i].getY();
		currX = pos[i].getX();
		pos[i].draw(symbol, backgroundcolor);
		board->board[currY][currX] = symbol;
	}
	hideCursor();
}
size_t Ship::getTrunkWeight() const
{
	size_t carryWeight = 0;
	for(const auto& block : trunk)
	{
		carryWeight += block.second->getSize();
	}
	return carryWeight;
}