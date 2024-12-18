#pragma once

#include "gameConfig.h"
#include "utils.h"

class Health
{
	// Health Details
	int x;
	int y;
	size_t livesLeft = GameConfig::LIVES;
	char healthSymbol = GameConfig::HEALTH_SYMBOL;
	bool isColor = false;

public:
	// getters
	size_t getlivesLeft() { return livesLeft; }

	// setters
	void setLocation(int x, int y) { this->x = x; this->y = y; }
	void setColor(bool isColor) { this->isColor = isColor; }

	// modifiers and checkers
	void decreaseLife() { --livesLeft; }
	bool isAlive();
	void printHealth();
};

