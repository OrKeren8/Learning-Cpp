#define _CRT_SECURE_NO_WARNINGS

#include "ResultIO.h"
#include "GameSleep.h"

#include "string"


void ResultIO::writeEvent(size_t timeLeft, Events event) {
	loadFileByMode();
	string message;
	message.append(std::to_string((int)timeLeft));
	message.append(" ");
	message.append(std::to_string((int)event)); // 1 is Win, 0 is Death
	wfp.getFile() << message << std::endl;
}


void ResultIO::setMode(FileMode _mode) {
	mode = _mode;
	loadFileByMode();
}

void ResultIO::loadFileByMode() 
{
	if (currLevel != level) {
		currLevel = level;
		switch (mode) {
		case FileMode::write:
			rfp.close();
			wfp.close();
			GameSleep::systemOprSleep();
			wfp.open(getFileName());
			break;
		case FileMode::read:
			rfp.close();
			wfp.close();
			GameSleep::systemOprSleep();
			rfp.open(getFileName());
			break;
		default:
			throw std::exception("Chosen mode not exist");
			break;
		}
	}
}

bool ResultIO::cmpEvents(size_t _timeLeft, Events _event) {
	bool res = false;
	string line;
	int event, timeLeft;

	loadFileByMode();
	getline(rfp.getFile(), line);
	std::sscanf(line.c_str(), "%d %d", &timeLeft, &event);

	if (timeLeft == _timeLeft and event == (size_t)_event) {
		res = true;
	}
	return res;
}