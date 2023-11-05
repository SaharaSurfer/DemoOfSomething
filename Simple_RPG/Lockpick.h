#pragma once
#include <random>
#include "Item.h"
#include "Lock.h"
#include "Player.h"

class Lockpick : public Item
{
private:
	std::default_random_engine generator;

public:
	Lockpick();

	bool UnlockObject(Lock& object, Player& player);
};