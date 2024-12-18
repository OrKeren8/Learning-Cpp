#pragma once
#include "StepInput.h"

#include <conio.h>


class KeyboardInput : public StepInput
{
public:
	virtual char getAction() const override {return _getch();}
	virtual bool hasInput() override {return _kbhit();}
};
