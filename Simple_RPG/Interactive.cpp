#include "Interactive.h"

int Interactive::GetLockLevel()
{
	return lock_level;
}

void Interactive::SetLockLevel(int level)
{
	if (level >= 0 and level <= 20)
	{
		lock_level = level;
	}
}