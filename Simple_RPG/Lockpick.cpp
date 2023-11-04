#include "Lockpick.h"

Lockpick::Lockpick() 
{
	LoadData("Lockpick");
}

bool Lockpick::UnlockObject(Lock& object, Player& player)
{
	std::srand(time(0));

	int player_toss = rand() % 20 + 1 + player.GetStatBonus("DEX");
	
	if (player_toss >= object.GetLockLevel())
	{
		object.SetLockLevel(0);
		return 1;
	}
	
	return 0;
}
