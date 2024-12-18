#include "health.h"

#include "utils.h"
#include "GamePrint.h"



bool Health::isAlive() 
{
	if (livesLeft <= 0)
		return false;
	return true;
}




void Health::printHealth() {
	gotoxy(x + GameConfig::MIN_X, y + GameConfig::MIN_Y);

	GamePrint::print("Remaining Lives: ", true);
	
	if (isColor)
		setTextColor(GameConfig::LIGHTRED);
 
	GamePrint::print(to_string(this->livesLeft), true);

	setTextColor(GameConfig::WHITE);
	hideCursor();
}