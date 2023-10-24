#pragma once
#include "Interface.h"
#include "Player.h"

class GamePlayHandler
{
private:
	Interface interface;

	Race ChoosingRace();
	GameClass ChoosingClass();
public:
	void ProceedCharacterCreation();
};