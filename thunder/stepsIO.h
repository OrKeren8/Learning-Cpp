#pragma once

#include "SmartFile.h"
#include "smartReadFile.h"
#include "SmartWriteFile.h"
#include "StepInput.h"
#include "gameConfig.h"

#include <string>


using std::string;


class StepsIO: public StepInput{
public:
	enum class FileMode { read = 0, write };
private: 
	// StepsIO Details
	FileMode mode = FileMode::read;
	const string stepsEnding = ".steps.txt", prefix = "tb0";
	const size_t& currTime, & level;
	size_t timeStamp = GameConfig::GAME_TIME + 1;
	char currAction = 0;
	size_t currLevel = 0;
	bool is_silent = false;

	// stepsIO Objects
	smartReadFile rfp;
	SmartWriteFile wfp;

	// private functions (helpers)
	void loadFileByMode();
	string getFileName() const {return prefix + std::to_string(level) + stepsEnding; }
public:
	StepsIO(const size_t& currTime, const size_t& level) :currTime(currTime), level(level) { loadFileByMode(); currLevel = 0; }
	
	// getters
	virtual char getAction() const override {return currAction; }
	
	// setters
	void setSilent(bool silent) { is_silent = silent; }
	void setMode(FileMode _mode);

    // modifiers and checkers
	virtual bool hasInput() override;
	void writeStep(size_t step, size_t timeLeft);
};
