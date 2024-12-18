#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

#include "gameConfig.h"
#include "smartReadFile.h"

using std::string;
using std::vector;

class Mapsfiles
{
	// map legend
	char legend[3][GameConfig::GAME_WIDTH + 1] = {
		"W T                                                       WWWW            W    W",
		"W                                                         WBBW           WSW   W",
		"W L                                                       WBBW           WSW   W"
	};
	// Mapsfiles Details
	vector<string> filesNames;
	size_t fileIndex = 0;
	string filesPath = "mapFiles";
	string currfileName;
	string fileSuffix = ".screen.txt";
	bool fileStatus = false;
	bool currlevelLoaded = false;
	smartReadFile fileMap;

	// private functions (helpers)
	void checkFileStatus();
	void copyHeaderToMap(char map[][GameConfig::GAME_WIDTH + 1],size_t& line,size_t& col);
	bool checkMapAndUpdate(char map[][GameConfig::GAME_WIDTH + 1]);
	void GetUserFileChoice();
	
	
public:
	// getters
	bool getMap(char map[][GameConfig::GAME_WIDTH + 1],bool userChoice);
	bool getCurrLevelLoadedStatus() const { return currlevelLoaded; }
	void getlevel(size_t& index) const { index = fileIndex + 1; }
	bool loadMapLevels(size_t& levels);
	void loadNextMap();
};

