#pragma once
#include "GameEvent.h"
#include "Key.h"

class UseKeyEvent : public GameEvent
{
private:
	Lock CreateLock(json node);
	bool UnlockWithKey(Lock& lock, Character& player, Interface& interface, json node);

public:
	int Execute(json node, Character& player, Interface& interface) override;
};