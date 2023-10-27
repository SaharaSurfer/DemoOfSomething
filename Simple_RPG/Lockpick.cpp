#include "Lockpick.h"

Lockpick::Lockpick() 
{
	LoadData("Lockpick");
}

void Lockpick::UnlockObject(Interactive& object, Player& player)
{
	std::srand(time(0));

	int player_toss = rand() % 20 + player.GetDexBonus();
	
	if (player_toss >= object.GetLockLevel())
	{
		object.SetLockLevel(0);
	}
}
