#pragma once
#include "Interface.h"
#include "Player.h"

class GamePlayHandler
{
private:
	Interface interface;
	Player player;

	Race ChoosingRace();
	GameClass ChoosingClass();
public:
	GamePlayHandler();

	void ProceedCharacterCreation();
	void TellJourneyStart();
};