#pragma once
#include <random>
#include "Item.h"
#include "Lock.h"
#include "Character.h"

class Lockpick : public Item
{
private:
	std::default_random_engine generator;

public:
	Lockpick();

	bool UnlockObject(Lock& object, Character& player);
};