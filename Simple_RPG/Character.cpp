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

int Character::GetDexBonus()
{
	std::map<int, int> bonuses
	{
		{10, 0}, {11, 1}, {12, 2},
		{13, 3}, {14, 3}, {15, 3},
		{16, 3}, {17, 4}, {18, 4},
		{19, 5}, {20, 6}
	};
	
	return bonuses[dexterity];
}

std::string Character::GetCharacterStats()
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
	return text;
}

std::string Character::GetInventoryStats()
{
	std::string text = "";
	for (int i = 0; i < inventory.size(); i++)
	{
		text += inventory[i] -> GetName() + ", ";
	}
	return text;
}

void Character::AddItemToInventory(Item* object)
{
	inventory.push_back(object);
}

void Character::DeleteItemFromInventory(std::string name)
{
	for (int i = 0; i < inventory.size(); i++)
	{
		if (inventory[i] -> GetName() == name)
		{
			inventory.erase(inventory.begin() + i);
			break;
		}
	}
}

Item* Character::GetItemFromInventory(std::string name) 
{
	for (Item* item : inventory)
	{
		if (item -> GetName() == name)
		{
			return item;
		}
	}
}

int Character::CountItemInInventory(std::string name)
{
	int cnt = 0;
	for (Item* item : inventory)
	{
		if (item -> GetName() == name)
		{
			cnt++;
		}
	}
	return cnt;
}