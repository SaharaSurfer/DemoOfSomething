#pragma once
#include "Item.h"
#include "Lock.h"
#include "Player.h"

class Lockpick : public Item
{
private:

public:
	Lockpick();

	bool UnlockObject(Lock& object, Player& player);
};