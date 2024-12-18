#define _CRT_SECURE_NO_WARNINGS

#include "stepsIO.h"
#include "gameConfig.h"
#include "GameSleep.h"

#include <string>
#include <conio.h>

using std::to_string;


void StepsIO::setMode(FileMode _mode) {
	mode = _mode;
	loadFileByMode();
}

void StepsIO::loadFileByMode() {
	if (currLevel != level) {
		currLevel = level;
		timeStamp = GameConfig::GAME_TIME + 1;

		switch (mode) {
		case FileMode::write:
			rfp.close();
			wfp.close();
			//GameSleep::systemOprSleep();
			wfp.open(getFileName());
			break;
		case FileMode::read:
			rfp.close();
			wfp.close();
			//GameSleep::systemOprSleep();
			rfp.open(getFileName());
			break;
		default:
			throw std::exception("Chosen mode not exist");
			break;
		}
	}
}

void StepsIO::writeStep(size_t step, size_t timeLeft)
{
	loadFileByMode();
	string message = to_string(step) + " " + to_string(timeLeft);
	if (GameConfig::isShipControlMove((GameConfig::eKeys)step) or step==0)
		wfp.getFile() << message << std::endl;;
}

// if ESC was pressed, the function will return true
bool cmdInterrupt(bool silent) {
	bool interrupt = false;
	if (!silent &&_kbhit())
		if ((GameConfig::eKeys)_getch() == GameConfig::eKeys::ESC)
			interrupt = true;
	return interrupt;
}

// this function checks if there is an new input in the file according to the current time
bool StepsIO::hasInput() {
	loadFileByMode();
	if (currTime == GameConfig::GAME_TIME) {
		timeStamp = GameConfig::GAME_TIME + 1;
	}
	bool res = false;
	if (cmdInterrupt(is_silent)) {
		throw std::ios_base::failure("Esc");
	}
	else if (timeStamp == currTime) {
		res = true;
	}
	else if (timeStamp > currTime) {
		string line;
		getline(rfp.getFile(), line);
		std::sscanf(line.c_str(), "%d %d", &currAction, &timeStamp);
		if (timeStamp == currTime)
			res = true;
	}
	return res;
}