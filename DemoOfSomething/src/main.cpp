#include "../header/GamePlayHandler.h"

int main()
{
	Interface& interface = Interface::GetInstance();
	GamePlayHandler game_engine(interface);

	game_engine.CreatePlayerCharacter();
	game_engine.EscapeFromCage();
	
	interface.LoadLocation("Locations\\GameOver.txt");
	interface.RenderLocation();

	return 0;
}