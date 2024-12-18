#include "gameConfig.h"


const char GameConfig::SHIPS_SYMBOLS[] = {'#', '@'};
const int GameConfig::SHIPS_CARRY_WEIGHT[] = { 6, 2 };
const GameConfig::Color GameConfig::SHIPS_COLORS[][NUM_SHIPS] = {{WHITE, WHITE}, {PURPLE, TURQUOISE}};
bool GameConfig::isShipControlMove(GameConfig::eKeys key) {
	if (key == eKeys::LEFT || key == eKeys::RIGHT || key == eKeys::UP || key == eKeys::DOWN ||
		key == eKeys::SWITCH_TO_SMALL_S || key == eKeys::SWITCH_TO_BIG_S)
		return true;
	return false;
}