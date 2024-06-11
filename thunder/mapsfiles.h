#pragma once
#include <string>
#include "gameConfig.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
using std::string;
using std::ifstream;
using std::vector;
class Mapsfiles
{
	char legend[3][GameConfig::GAME_WIDTH + 1] = {
		"W T                                                       WWWW            W    W",
		"W                                                         WBBW           WSW   W",
		"W L                                                       WBBW           WSW   W"
	};
	vector<string> filesNames;
	size_t fileIndex = 0;
	string filesPath = "mapFiles";
	string currfileName;
	string fileSuffix = ".screen.txt";
	bool fileStatus = false;
	bool mapsLoaded = false;
	bool currlevelLoaded = false;
	ifstream fileMap;
	void checkFileStatus();
	void copyHeaderToMap(char map[][GameConfig::GAME_WIDTH + 1],size_t& line,size_t& col);
	bool checkMapAndUpdate(char map[][GameConfig::GAME_WIDTH + 1]);
	void GetUserFileChoice();
	void loadNextMap();
	
public:
	void loadMapLevels();
	//~Mapsfiles();
	bool getMap(char map[][GameConfig::GAME_WIDTH + 1],bool userChoice);
	bool getMapsLoadedstatus() const { return mapsLoaded; }
	bool getCurrLevelLoadedStatus() const { return currlevelLoaded; }
	
};

