#include "Lock.h"

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