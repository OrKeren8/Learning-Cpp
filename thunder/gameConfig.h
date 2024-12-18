#ifndef __GAME_CONFIG_H
#define __GAME_CONFIG_H

#include <Windows.h>


class GameConfig
{
public:
	enum class eKeys { LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 'x', SWITCH_TO_SMALL_S = 's', SWITCH_TO_BIG_S = 'b', ESC = 27, EXIT = (int)'9' };
	static constexpr int GAME_WIDTH = 80;
	static constexpr int GAME_HEIGHT = 25;

	static constexpr int MIN_X = 10;
	static constexpr int MIN_Y = 3;

	static constexpr char BIG_SHIP_S = '#';
	static constexpr char SMALL_SHIP_S = '@';
	static constexpr char FINISH_S = 'X';
	static constexpr char LEGEND_S = '&';
	static constexpr char FINISH_BIG_SHIP = 'B';
	static constexpr char FINISH_SMALL_SHIP = 'S';
	static constexpr char TIME_SYMBOL = 'T';
	static constexpr char HEALTH_SYMBOL = 'L';
	static constexpr char WALL_SYMBOL = 'W';

	static constexpr int BIG_SHIP_ID = 0;
	static constexpr int SMALL_SHIP_ID = 1;


	static constexpr size_t LONG_SLEEP = 4000;
	static constexpr size_t SHORT_SLEEP = 1500;
	static constexpr size_t SYSTEM_OPR_SLEEP = 80;
	static constexpr size_t GAME_SPEED = 80; 
	static constexpr size_t MIN_TIME = 1;
	static constexpr size_t GAME_TIME = 400;
	static constexpr size_t LIVES = 3;
	
	static bool isShipControlMove(GameConfig::eKeys key);

	enum Color
	{
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		MAGENTA = 5,
		BROWN = FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN,
		LIGHTGREY = 7,
		DARKGREY = 8,
		LIGHTBLUE = 9 | 9 << 4,
		LIGHTGREEN = 10,
		LIGHTCYAN = 11,
		LIGHTRED = 12,
		LIGHTMAGENTA = 13,
		YELLOW = 14,
		WHITE = 15,
		WHITE_BACKGROUND = 15 << 4,
		BACKGROUND_GREY = 8<<4 | 8,
		PURPLE = BACKGROUND_BLUE | BACKGROUND_RED | BLUE | RED,
		TURQUOISE = BACKGROUND_BLUE | BACKGROUND_GREEN | BLUE | GREEN

	};

	static constexpr GameConfig::Color WALL_COLOR = BACKGROUND_GREY;

	enum gamePieceType {
		PRIMARY = 0,
		SECONDARY = 1
	};


	static const size_t NUM_SHIPS = 2;
	static constexpr size_t MAX_SHIP_SIZE = 4;
	static const char SHIPS_SYMBOLS[NUM_SHIPS];
	static const int SHIPS_CARRY_WEIGHT[NUM_SHIPS];
	static const Color SHIPS_COLORS[NUM_SHIPS][NUM_SHIPS];
	enum ShipID {
		BIG = 0,
		SMALL = 1
	};
	

	static constexpr size_t MAX_NUM_BLOCKS = 26;
	static constexpr size_t MAX_BLOCK_SIZE = 10;
	static const Color BLOCK_COLOR = BROWN;

	enum ErrorCode {
		Success = 0,
		Failed = 1
	};
};
#endif