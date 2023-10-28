#pragma once
#include "Item.h"

class Lock : public Item
{
private:
	int lock_level = 0;
public:
	Lock();

	int GetLockLevel();
	void SetLockLevel(int level);
};