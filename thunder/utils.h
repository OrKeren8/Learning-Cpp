#ifndef UTILS_H
#define UTILS_H

#include <windows.h> // for gotoxy
#include <process.h> // for system
#include <iostream>

#include "gameConfig.h"
#include "point.h"

using namespace std;


void gotoxy(int, int);
void setTextColor(GameConfig::Color color);
void hideCursor();
void clrscr();


struct Coord {
	int x = -1;
	int y = -1;
};

struct LocationInfo {
	Point* nextPos = nullptr;
	char objSymbol = ' ';
	size_t objSize = 0;
	GameConfig::eKeys direction;
	int* carryWeight = nullptr;

};


#endif // __GENERAL_H