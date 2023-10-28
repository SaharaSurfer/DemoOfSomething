#pragma once
#include "Interface.h"
#include "Player.h"
#include "Lockpick.h"
#include "Lock.h"

class GamePlayHandler
{
private:
	Interface interface;
	Player player;

	Race ChoosingRace();
	GameClass ChoosingClass();

	int HandleEvent(json node);
	int ReceiveItems(json node);
	int ProcessLockpicking(json node);

	void ProcessDecisionTree(json tree);
public:
	GamePlayHandler();

	void CreatePlayerCharacter();
	void EscapeFromCage();
};