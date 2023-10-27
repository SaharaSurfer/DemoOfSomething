#pragma once

class Interactive
{
private:
	int lock_level = 0;
public:
	int GetLockLevel();
	void SetLockLevel(int level);
};