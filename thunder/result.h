/*
#pragma once
#include "gameConfig.h"
#include "GamePlay.h"

#include <string>

using std::string;


class Result : public GamePlay
{
	enum class Events {
		DEATH,
		FINISH_LEVEL
	};
	
	const string resultEnding = ".result";


public:
	Result() { ending = resultEnding; }
	void writeEvent(int timeLeft, Events event);
};

*/