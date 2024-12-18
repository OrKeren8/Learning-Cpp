#include "time.h"
#include "iostream"
#include "utils.h"
#include "gameConfig.h"
#include "GamePrint.h"


void Time::setTimeSettings(const size_t totalTime, const bool isColor)
{
	this->totalTime = timeLeft = totalTime;
	this->isColor = isColor;
	printTime(totalTime / 10);
}

bool Time::checkAndUpdateTime()
{
	timeLeft--;
	if (timeLeft <= 1)
	{
		resetTime();
		return true;
	}
	else
	{
		if (timeLeft % 10 == 0)
			printTime(timeLeft / 10);
		return false;
	}
}

void Time::printTime(size_t time)
{
	gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);
	GamePrint::print("Time Left : ", true);
	if (isColor)
		setTextColor(GameConfig::GREEN);

	// Print updated time
	for (int i = 0; i < time; i++)
	{
		GamePrint::printChar('|');
	}

	// Overwrite the remaining characters with spaces
	for (int i = time; i < (totalTime / 10); i++)
	{
		GamePrint::printChar(' ');
	}
	setTextColor(GameConfig::WHITE);
	hideCursor();
}