#include "Lockpick.h"

Lockpick::Lockpick() : Item(), generator(std::random_device{}())
{
	LoadData("Lockpick");
}
	
bool Lockpick::UnlockObject(Lock& object, Character& player)
{
	std::uniform_int_distribution<int> distribution(1, 20);
	int player_toss = distribution(generator) + player.characteristics.GetStatBonus("DEX");
	
	if (player_toss >= object.GetLockLevel())
	{
		object.SetLockLevel(0);
		return true;
	}
	
	return false;
}
