#pragma once
#include "BattleEvent.h"
#include "DialogEvent.h"
#include "LockpickingEvent.h"
#include "ReceiveItemsEvent.h"
#include "SurpriseAttackEvent.h"
#include "UseKeyEvent.h"

class EventHandler
{
private:
	Interface& interface;
	std::unordered_map<std::string, std::unique_ptr<GameEvent>> event_handlers;

public:
	EventHandler(Interface& intf);

	int HandleEvent(json node, Character& player);
};