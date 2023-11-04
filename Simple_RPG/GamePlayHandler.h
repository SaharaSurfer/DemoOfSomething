#pragma once
#include "Interface.h"
#include "EventHandler.h"
#include "Player.h"
#include "NPC.h"
#include "Lockpick.h"
#include "Key.h"
#include "Lock.h"

class GamePlayHandler
{
private:
	Interface interface;
	EventHandler event_handler;
	Player player;

	Race ChoosingRace();
	GameClass ChoosingClass();

	void ProcessDecisionTree(json tree);
public:
	GamePlayHandler();

	void CreatePlayerCharacter();
	void EscapeFromCage();
};