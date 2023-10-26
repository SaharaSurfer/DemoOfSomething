#include "Character.h"

Character::Character() {}

Character::Character(Race r, GameClass gc)
{
	race = r;
	gameclass = gc;

	strength += r.strengthBonus;
	dexterity += r.dexterityBonus;
	intelligence += r.intelligenceBonus;
	wisdom += r.wisdomBonus;
	charisma += r.charismaBonus;

	health += gc.healthBonus * level;
	mana += gc.manaBonus * level;
	defense += gc.defenseBonus * level;
	attack += gc.attackBonus * level;
}

void Character::ShowStats()
{
	std::string text = 
		"Race - " + race.name 
		+ "\nClass - " + gameclass.name
		+ "\n-------------------------------"
		+ "\nHealth = " + std::to_string(health) 
		+ "\nMana = " + std::to_string(mana)
		+ "\nDefense = " + std::to_string(defense) 
		+ "\nAttack = " + std::to_string(attack)
		+ "\n-------------------------------"
		+ "\nStrength = " + std::to_string(strength)
		+ "\nDexterity = " + std::to_string(dexterity)
		+ "\nIntelligence = " + std::to_string(intelligence)
		+ "\nWisdom = " + std::to_string(wisdom)
		+ "\nCharisma = " + std::to_string(charisma);
	std::cout << text.c_str();
}