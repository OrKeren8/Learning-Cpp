#ifndef UTILS_H
#define UTILS_H

#include <windows.h> // for gotoxy
#include <process.h> // for system
#include <iostream>
#include "Color.h"
using namespace std;


void gotoxy(int, int);
void setTextColor(Color);
void hideCursor();
void clrscr();

#endif // __GENERAL_H