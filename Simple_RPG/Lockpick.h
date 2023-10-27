#pragma once
#include "Item.h"
#include "Interactive.h"
#include "Player.h"

class Lockpick : public Item
{
private:

public:
	Lockpick();

	void UnlockObject(Interactive& object, Player& player);
};