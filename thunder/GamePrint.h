#pragma once

#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::ostream;


class GamePrint {
public:
	static bool silentMode;
	static void print(const std::string& data, bool inLine = false) 
	{
		if (!silentMode)
		{
			cout << data;
			if (!inLine)
				cout << endl;
		}
	}
	static void printChar(const char c) 
	{
		if (!silentMode)
		cout << c;
	}
	static void moveToMiddle()
	{
		if(!silentMode)
			for (int i = 1; i <= 34; i++)
				printChar(' ');
	}
};