#include "../header/GamePlayHandler.h"

int main()
{
	GamePlayHandler game_engine;

	game_engine.CreatePlayerCharacter();
	game_engine.EscapeFromCage();

	return 0;
}