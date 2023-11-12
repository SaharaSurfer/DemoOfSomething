#include "../header/EventHandler.h"

EventHandler::EventHandler(Interface& intf) : interface(intf) 
{
	event_handlers["ReceiveItems"] = std::make_unique<ReceiveItemsEvent>();
	event_handlers["ProcessLockpicking"] = std::make_unique<LockpickingEvent>();
	event_handlers["UseKeyToOpen"] = std::make_unique<UseKeyEvent>();
	event_handlers["ProcessDialog"] = std::make_unique<DialogEvent>();
	event_handlers["ProcessSurpriseAttack"] = std::make_unique<SurpriseAttackEvent>();
	event_handlers["ProcessFight"] = std::make_unique<BattleEvent>();
}

int EventHandler::HandleEvent(json node, Character& player)
{
	const std::string event_type = node["event"];
	auto it = event_handlers.find(event_type);
	if (it != event_handlers.end()) 
	{
		return it->second->Execute(node, player, interface);
	}

	return 0;
}
