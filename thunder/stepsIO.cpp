#define _CRT_SECURE_NO_WARNINGS

#include "stepsIO.h"
#include "gameConfig.h"

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
			Sleep(3000);
			wfp.open(getFileName());
			break;
		case FileMode::read:
			wfp.close();
			Sleep(3000);
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
	string message = to_string(step) + " " + to_string(timeLeft);
	if (GameConfig::isShipControlMove((GameConfig::eKeys)step))
		wfp.getFile() << message << std::endl;;
}

bool cmdInterrupt() {
	bool interrupt = false;
	if (_kbhit())
		if ((GameConfig::eKeys)_getch() == GameConfig::eKeys::ESC)
			interrupt = true;
	return interrupt;
}

bool StepsIO::hasInput() {
	loadFileByMode();
	bool res = false;
	if (cmdInterrupt()) {
		currAction = (char)GameConfig::eKeys::ESC;
		res = true;
	}
	else if (timeStamp == currTime)
		res = true;
	else if (timeStamp > currTime) {
		string line;
		getline(rfp.getFile(), line);
		std::sscanf(line.c_str(), "%d %d", &currAction, &timeStamp);
	}
	return res;
}