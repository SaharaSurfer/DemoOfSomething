#pragma once
#include "GameEvent.h"
#include "Lockpick.h"

class LockpickingEvent : public GameEvent
{
private:
	Lock CreateLock(json node);
	int AttemptToUnlock(Lock& lock, Character& player, Interface& interface, json node);
	void HandleFailure(std::shared_ptr<Lockpick> lockpick, Character& player, Interface& interface, json node);

public:
	int Execute(json node, Character& player, Interface& interface) override;
};