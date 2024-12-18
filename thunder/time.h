#pragma once

class Time
{
	// Time Details
	int x;
	int y;
	size_t totalTime;
	size_t timeLeft;
	char timeSymbol;
	bool isColor = false;

public:
	// getters
	const size_t& getTimeLeft() const { return timeLeft;}

	// setters
	void setLocation(int x, int y) { this->x = x; this->y = y; }
	void setTimeSettings(const size_t totalTime, const bool isColor);

	// modifiers and checkers
	bool checkAndUpdateTime();
	void resetTime() { timeLeft = totalTime; }
	void printTime(size_t time);
	void reverse() { totalTime += 1; }
};

