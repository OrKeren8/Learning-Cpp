#pragma once


class StepInput
{
	/**
	* Acstract Base class for the key inputs for the game
	*/

public:
	virtual bool hasInput() = 0;
	virtual char getAction() const = 0;
	virtual ~StepInput() = default;
};

