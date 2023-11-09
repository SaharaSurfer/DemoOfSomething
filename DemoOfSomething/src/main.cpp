#include "../header/GamePlayHandler.h"

int main()
{
	GamePlayHandler game_engine;

	game_engine.CreatePlayerCharacter();
	game_engine.EscapeFromCage();
	
	Interface interface;
	interface.LoadLocation("Locations\\GameOver.txt");
	interface.RenderLocation();

	return 0;
}