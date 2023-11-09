#pragma once
#include <unordered_map>
#include <functional>
#include <deque>
#include "Interface.h"
#include "Character.h"
#include "Lockpick.h"
#include "Key.h"
#include "Lock.h"
#include "BattleHandler.h"

class EventHandler
{
private:
	Interface& interface = Interface::GetInstance();

	int ReceiveItems(json node, Character& player);
	int ProcessLockpicking(json node, Character& player);
	int UseKeyToOpen(json node, Character& player);
	int ProcessDialog(json node, Character& player);
	int ProcessSurpriseAttack(json node, Character& player);
	int ProcessFight(json node, Character& player);

public:
	int HandleEvent(json node, Character& player);
};