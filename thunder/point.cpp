#include "point.h"

#include <iostream>
#include <Windows.h>
#include "utils.h"
#include "gameConfig.h"
#include "GamePrint.h"


void Point::set(int x, int y)
{
	this->x = x;
	this->y = y;
}


/**
 * Draws the point on the screen.
 *
 * @param symbol The character representing the point.
 * @param backcolor The background color of the point.
 */
void Point::draw(char symbol,GameConfig::Color backcolor)
{
	setTextColor(backcolor);
	gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);
	GamePrint::printChar(symbol);
	setTextColor(GameConfig::WHITE);
}


/**
 * Moves the point based on the provided direction key.
 * 
 * If the point exceeds the boundaries of the game area,
 * it wraps around to the opposite side.
 *
 * @param key: indicating the movement direction.
 */
void Point::move(GameConfig::eKeys key)
{
	switch (key)
	{
	case GameConfig::eKeys::LEFT:
		diff_x = -1;
		diff_y = 0;
		break;
	case GameConfig::eKeys::RIGHT:
		diff_x = 1;
		diff_y = 0;
		break;
	case GameConfig::eKeys::UP:
		diff_x = 0;
		diff_y = -1;
		break;
	case GameConfig::eKeys::DOWN:
		diff_x = 0;
		diff_y = 1;
		break;
	}

	x += diff_x;
	if (x >= GameConfig::GAME_WIDTH)
		x = 0;
	else if (x < 0)
		x = GameConfig::GAME_WIDTH -1;

	y += diff_y;
	if (y >= GameConfig::GAME_HEIGHT)
		y = 1;
	else if (y == 1)
		y = GameConfig::GAME_HEIGHT;
}
