#pragma once
#include "Item.h"

class Lock : public Item
{
private:
	int lock_level = 0;

	std::string key_name = "";

public:
	Lock();

	int GetLockLevel();
	void SetLockLevel(int level);

	std::string GetKeyName();
	void SetKeyName(std::string k_name);
};