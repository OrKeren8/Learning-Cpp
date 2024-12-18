#include "board.h"
#include "gameConfig.h"
#include "utils.h"
#include "GamePrint.h"
#include <set>
/**
 * Initializes the game board and its components.
 *
 * This function initializes the game board and its components, including ships, blocks,
 * the exit, and the legend position. It then updates the game pieces on the board and
 * prints the initial state of the game screen.
 */
void Board::init(bool colorSet)
{
	this->colorSet = colorSet;
	size_t colorShift = 0;
	if (colorSet)
		colorShift++;

	for (size_t i = 0; i < GameConfig::NUM_SHIPS; i++)
		ships[i].init(GameConfig::SHIPS_SYMBOLS[i], GameConfig::SHIPS_CARRY_WEIGHT[i], GameConfig::SHIPS_COLORS[colorShift][i], this);
	updateGamePieces();
	printScreen();
	
}


/**
 * Prints the current state of the game board on the screen with appropriate indentation.
 */
void Board::printScreen()
{
	GameConfig::Color color = GameConfig::WHITE;
	for (size_t j = 0; j < GameConfig::MIN_Y; j++)
		GamePrint::print("");
	for (int i = 0; i < HEIGHT; i++) {
		for (size_t j = 0; j < GameConfig::MIN_X; j++)
			GamePrint::printChar(' ');
		for (size_t j = 0; j < WIDTH; j++)
		{
			char currSymbol = board[i][j];
			if (colorSet)
			{
				switch (currSymbol)
				{
				case GameConfig::BIG_SHIP_S:
					color = ships[GameConfig::BIG_SHIP_ID].getBackgroundColor();
					break;
				case GameConfig::SMALL_SHIP_S:
					color = ships[GameConfig::SMALL_SHIP_ID].getBackgroundColor();
					break;
				case GameConfig::WALL_SYMBOL:
					color = GameConfig::WALL_COLOR;
					break;
				case GameConfig::FINISH_S:
					color = GameConfig::WHITE_BACKGROUND;
					break;
				case GameConfig::HEALTH_SYMBOL:
					health.setColor(colorSet);
					break;
				default:
					if (Block::isBlock(currSymbol))
						color = Block::getBackgroundColor();
					break;
				}
			}
			setTextColor(color);
			if(currSymbol == GameConfig::FINISH_BIG_SHIP || currSymbol == GameConfig::FINISH_SMALL_SHIP)
			{
				GamePrint::printChar(' ');
			}
			else
			{
				GamePrint::printChar(currSymbol);
			}
			setTextColor(color = GameConfig::WHITE);
		}
		GamePrint::print("");
	}
	
}

/**
 * Updates the game pieces on the board based on the current state.
 *
 * This function updates the positions of various game pieces such as ships, blocks, exit,
 * and legend position on the board according to the current state of the game.
 */
void Board::updateGamePieces()
{
	std::memcpy(board, original_board, sizeof(original_board));
	if(!colorSet)
		Block::setColor(GameConfig::WHITE);
	for (int i = 0; i < HEIGHT; i++) 
	{
		for (int j = 0; j < WIDTH; j++) 
		{
			char currSymbol = board[i][j];
            switch (currSymbol) 
			{
			    case GameConfig::TIME_SYMBOL:
                    time.setLocation(j, i);
                    break;
                case GameConfig::HEALTH_SYMBOL:
                    health.setLocation(j, i);
                    break;
				case GameConfig::BIG_SHIP_S:
                    ships[0].addPoint(j, i);
                    break;
                case GameConfig::SMALL_SHIP_S:
                    ships[1].addPoint(j, i);
                    break;
					case GameConfig::FINISH_S:
                    exit_pos.set(j, i);
                    break;
				case GameConfig::FINISH_BIG_SHIP:
					ships[0].addFinishPoint(j, i);
					break;
				case GameConfig::FINISH_SMALL_SHIP:
					ships[1].addFinishPoint(j, i);
					break;
                default:
                    if (Block::isBlock(currSymbol))
					{
						blocks.insert({ currSymbol, {currSymbol, this}}); //check in dbug if there is a copy ctor involved
                        blocks[currSymbol].addPoint(j, i);
                    }
                    break;
            }
		}
	}
}


bool Board::checkMove(LocationInfo &ol)
{
	int currY, currX;
	char currSymbol;
	bool isValid = true,isfinished = false;
	set <Block*> obsticals;
	for(int i=0; i<ol.objSize && isValid; i++)
	{
		currY = ol.nextPos[i].getY();
		currX = ol.nextPos[i].getX();

		currSymbol = board[currY][currX];
		if (currSymbol != ' ' && currSymbol != ol.objSymbol)
			if (currSymbol == GameConfig::WALL_SYMBOL)
				isValid = false;
			else if (currSymbol == GameConfig::FINISH_S && Ship::isShip(ol.objSymbol))
				isfinished = true;
			else if (Block::isBlock(currSymbol))
				obsticals.insert(&blocks[currSymbol]);
			else if (Block::isBlock(ol.objSymbol) && blocks[ol.objSymbol].getCarrierShipID() == currSymbol)
				isValid = true;
			else
				isValid = false; //if collide with other ship that is not the carrier
	}

	//check for the whole chunk if it can move
	for (auto obs : obsticals) {
		if (!isValid)
			break;
		isValid = obs->checkMove(ol.direction, ol.carryWeight);
	}
	
	if(isValid)
	{
		if (isfinished) {
			shipFinishLine(ol.objSymbol);
			isValid = false; // there is no reason to move after we finished 
		}
		else {
				//move the whole chunk togther
				for (auto obs : obsticals)
					obs->move(ol.direction, ol.carryWeight, true);
			
		}
	}
	return isValid;
}

bool Board::checkFall(LocationInfo& objLocationInfo, Block* cargoBlock, char keyCargoBlock)
{
	int currY, currX;
	char currSymbol;
	bool isValid = true;
	Ship* carryShip;
	bool stillCarried = false;
	map <char,Block*> obsticals;
	Block& currentBlock = blocks[objLocationInfo.objSymbol];
	isValid = checkBlockCrash(objLocationInfo,stillCarried,obsticals); // check if the block can move without crashing into other blocks or walls
	for (int i = 0; i < objLocationInfo.objSize && isValid; i++)
	{
		currY = objLocationInfo.nextPos[i].getY();
		currX = objLocationInfo.nextPos[i].getX();
		currSymbol = board[currY][currX];
		if(Ship::isShip(currSymbol))
		{
			carryShip = getShipBySymbol(currSymbol);
			if(carryShip != currentBlock.getCarrierShip() && currentBlock.isCarriedBlock()) // if we switched ships we remove it from the old ship and add it to the new one
				currentBlock.getCarrierShip()->removeFromTrunk(objLocationInfo.objSymbol, currentBlock);
			carryShip->addToTrunk(objLocationInfo.objSymbol, &currentBlock);
			stillCarried = true;
			isValid = false;
		}
	}
	for (auto& obs : obsticals)
	{
		if (obs.second->isCarriedBlock()) // if the block we fell on is carried we add the current block to the ship that carries it
		{
			stillCarried = true;
			carryShip = obs.second->getCarrierShip();
			carryShip->addToTrunk(objLocationInfo.objSymbol, &currentBlock);
		}
	}
	// if it was carried before and now someone pushed it, we need to remove it from the ship it was carried by
	if (currentBlock.isCarriedBlock() && !(stillCarried) && isValid)
	{
		ships[GameConfig::BIG_SHIP_ID].removeFromTrunk(objLocationInfo.objSymbol,currentBlock);
		ships[GameConfig::SMALL_SHIP_ID].removeFromTrunk(objLocationInfo.objSymbol,currentBlock);
	}
	if (isValid) //move the whole chunk togther
	{
		for (auto &obs : obsticals)
			obs.second->move(objLocationInfo.direction, objLocationInfo.carryWeight, true);
	}
	return isValid;
}

bool Board::checkBlockCrash(LocationInfo& objLocationInfo,bool& stillCarried, map <char, Block*>& obsticals)
{
	int currY, currX;
	char currSymbol;
	bool isValid = true;
	
	for (int i = 0; i < objLocationInfo.objSize ; i++)
	{
		currY = objLocationInfo.nextPos[i].getY();
		currX = objLocationInfo.nextPos[i].getX();
		currSymbol = board[currY][currX];
		if (currSymbol != ' ' && currSymbol != objLocationInfo.objSymbol)
			if (currSymbol == GameConfig::WALL_SYMBOL)
				return false;
		    else if (currSymbol == GameConfig::FINISH_S)
				return false;
		    else if (Block::isBlock(currSymbol))
				obsticals.insert({ currSymbol, &blocks[currSymbol]});
	}
	for (auto& obs : obsticals)
	{
		if (!isValid)
			return false;

		if (!obs.second->checkFall(&blocks[objLocationInfo.objSymbol], objLocationInfo.objSymbol))
		{
			isValid = false;
		}
	}
	return isValid;
}

void Board::shipFinishLine(char shipID)
{
	switch (shipID)
	{
	case GameConfig::BIG_SHIP_S:
		ships[0].shipFinishLine();
		break;
	case GameConfig::SMALL_SHIP_S:
		ships[1].shipFinishLine();
		break;
	}
}
void Board::resetBoard()
{
	health = Health();
	time = Time();
	blocks.clear();
	num_blocks = 0;
	for (size_t i = 0; i < GameConfig::NUM_SHIPS; i++)
		ships[i] = Ship();
	mapFileLoaded = false;
	life_pos = Point();
	exit_pos = Point();
	colorSet = false;
}
Ship* Board::getShipBySymbol(char sym)
{
	switch (sym)
	{
	case GameConfig::BIG_SHIP_S:
		return &(ships[GameConfig::BIG_SHIP_ID]);
	case GameConfig::SMALL_SHIP_S:
		return &(ships[GameConfig::SMALL_SHIP_ID]);
	default:
		throw exception("Can't find Ship by symbol");
	}
}