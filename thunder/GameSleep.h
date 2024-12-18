#pragma once
#include "gameConfig.h"

#include <Windows.h>


class GameSleep
{
	static constexpr DWORD longOprTime = GameConfig::LONG_SLEEP;
	static constexpr DWORD shortOprTime = GameConfig::SHORT_SLEEP;
	static constexpr DWORD systemOprTime = GameConfig::SYSTEM_OPR_SLEEP;
	static constexpr DWORD gameSpeed = GameConfig::GAME_SPEED;
	static constexpr DWORD minTime = GameConfig::MIN_TIME;

public:
	static bool silentMode;
	static void sleep(DWORD milliSec)
	{
		if (!silentMode)
			Sleep(milliSec);
	}
	static void longSleep() 
	{
		sleep(longOprTime);
	}
	static void shortSleep()
	{
		sleep(shortOprTime);
	}
	static void systemOprSleep() 
	{
	 	sleep(systemOprTime);	
	}

	static void gameOprSleep() 
	{
		sleep(gameSpeed);
	}
};

