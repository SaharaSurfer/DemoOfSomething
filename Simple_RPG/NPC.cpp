#include "NPC.h"

NPC::NPC() {}

NPC::NPC(const Race& r, const GameClass& gc, int lvl) : Character(r, gc)
{
	Interface interface;
	json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");
	for (size_t i = 0; i < list_of_abilities[gc.name].size() and i < lvl; i++)
	{
		for (json node : list_of_abilities[gc.name][i])
		{
			abilities.push_back(Ability(node));
		}
	}
}

bool NPC::IsDead()
{
	return health == 0;
}

std::string NPC::GetRaceName()
{
	return race.name;
}

std::string NPC::GetClassName()
{
	return gameclass.name;
}

std::vector<Ability> NPC::GetAbilities()
{
	return abilities;
}