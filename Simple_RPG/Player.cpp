#include "Player.h"

Player::Player() {}

Player::Player(Race r, GameClass gc)
{
	race = r;
	gameclass = gc;

	strength += r.strengthBonus;
	dexterity += r.dexterityBonus;
	intelligence += r.intelligenceBonus;
	wisdom += r.wisdomBonus;
	charisma += r.charismaBonus;

	max_health += gc.healthBonus * level;
	health = max_health;
	mana += gc.manaBonus * level;
	defense += gc.defenseBonus * level;
	attack += gc.attackBonus * level;


	Interface interface;
	json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");
	for (size_t i = 0; i < list_of_abilities[gc.name].size() and i < level; i++)
	{
		for (json node : list_of_abilities[gc.name][i])
		{
			abilities.push_back(Ability(node));
		}
	}
}

std::vector<Ability> Player::GetAbilities()
{
	return abilities;
}