#include "../header/Lock.h"

Lock::Lock() {}

int Lock::GetLockLevel()
{
	return lock_level;
}

void Lock::SetLockLevel(int level)
{
	if (level >= 0 and level <= 20)
	{
		lock_level = level;
	}
}

std::string Lock::GetKeyName()
{
	return key_name;
}

void Lock::SetKeyName(std::string k_name)
{
	key_name = k_name;
}