#pragma once

#include "SmartFile.h"
#include "smartReadFile.h"
#include "SmartWriteFile.h"
#include "gameConfig.h"

#include <string>


using std::string;

enum class Events {
	DEATH,
	FINISH_LEVEL
};

class ResultIO
{
public:
	enum class FileMode { read = 0, write };
private:
	// ResultIO Details
	FileMode mode = FileMode::read;
	smartReadFile rfp;
	SmartWriteFile wfp;
	const string stepsEnding = ".result.txt", prefix = "tb0";
	const size_t & level;
	size_t currLevel = 0;
	// private functions (helpers)
	void loadFileByMode();
	string getFileName() const { return prefix + std::to_string(level) + stepsEnding; }

public:
	ResultIO(const size_t& level): level(level) {}

	// setters
	void setMode(FileMode _mode);

	// modifiers and checkers
	void writeEvent(size_t timeLeft, Events event);
	bool cmpEvents(size_t timeLeft, Events event);
};
