#pragma once
#include "Lockpick.h"
#include "Lock.h"

class Key : public Lockpick
{
private:

public:
	Key();

	bool UnlockObject(Lock& object);
};