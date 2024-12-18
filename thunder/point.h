#ifndef POINT_H
#define POINT_H

#include "gameConfig.h"

using namespace std;


class Point
{
	// Point Details
	int x = -1, y = -1;
	int diff_x = 0;
	int diff_y = 0;

public:
	// getters
	int getX() const { return this->x; }
	int getY() const { return this->y; }
	void getCoordinate(int& x, int& y) const {x = this->x; y = this->y;}
	
	// setters
	void set(int x, int y);

	// modifiers
	void draw(char symbol, GameConfig::Color backcolor);
	void move(GameConfig::eKeys key);
	bool operator==(const Point& other) const {
		if ((this->x == other.x) && (this->y == other.y))
			return true;
		else
			return false;
	}
};

#endif