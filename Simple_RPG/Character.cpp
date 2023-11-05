#include "Character.h"

Character::Character() {}

Character::Character(const Race& r, const GameClass& gc) : race(r), gameclass(gc)
{
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

	LoadAbilitiesFromJSON();
}

void Character::LoadAbilitiesFromJSON()
{
	Interface interface;
	const json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");

	for (size_t i = 0; i < list_of_abilities[gameclass.name].size() and i < level; i++)
	{
		for (const auto& node : list_of_abilities[gameclass.name][i])
		{
			abilities.push_back(Ability(node));
		}
	}
}

std::string Character::GetRaceName()
{
	return race.name;
}

std::string Character::GetClassName()
{
	return gameclass.name;
}

std::string Character::GetName()
{
	return name;
}

void Character::SetName(const std::string& new_name)
{
	name = new_name;
}

void Character::GainExp(int exp)
{
	static std::map<int, int> exp_progression =
	{
		{1, 250}, {2, 300}, {3, 500}
	};

	experience += exp;

	if (experience >= exp_progression[level])
	{
		experience %= exp_progression[level];
		level++;

		Interface interface;
		const json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");
		for (const auto& node : list_of_abilities[gameclass.name][level - 1])
		{
			abilities.push_back(Ability(node));
		}
	}
}

int Character::GetHealthPercent()
{
	return (int)((float)health / max_health * 100);
}

int Character::GetMaxHealth()
{
	return max_health;
}

int Character::GetHealth()
{
	return health;
}

int Character::GetAttack()
{
	return attack;
}

int Character::GetMana()
{
	return mana;
}

int Character::GetEnergy()
{
	return energy;
}

void Character::IncreaseHealth(int shift)
{
	if (shift > 0)
	{
		health = std::min(health + shift, max_health);
	}
	else
	{
		float dmg_reduce = defense;
		for (std::pair<std::string, std::pair<int, int>> effect : positive_effects)
		{
			if (effect.first == "defense_boost" and effect.second.second != 0)
			{
				dmg_reduce *= (1 + effect.second.first / 100);
			}
		}

		if (dmg_reduce + shift < 0) 
		{
			health = std::max(health + shift + (int)dmg_reduce, 0);
		}
	}
}

void Character::IncreaseMana(int shift)
{
	if (shift > 0)
	{
		mana = std::min(mana + shift, max_mana);
	}
	else
	{
		mana = std::max(mana + shift, 0);
	}
}

void Character::IncreaseEnergy(int shift)
{
	if (shift > 0)
	{
		energy = std::min(energy + shift, max_energy);
	}
	else
	{
		energy = std::max(energy + shift, 0);
	}
}


int Character::GetStatBonus(std::string stat_name)
{
	std::map<int, int> bonuses
	{
		{10, 0}, {11, 1}, {12, 2},
		{13, 3}, {14, 3}, {15, 3},
		{16, 3}, {17, 4}, {18, 4},
		{19, 5}, {20, 6}
	};
	
	if (stat_name == "STR") { return bonuses[strength]; }
	if (stat_name == "DEX") { return bonuses[dexterity]; }
	if (stat_name == "INT") { return bonuses[intelligence]; }
	if (stat_name == "WIS") { return bonuses[wisdom]; }
	if (stat_name == "CHA") { return bonuses[charisma]; }

}

std::string Character::GetCharacterStats()
{
	std::string text =
		"Level - " + std::to_string(level)
		+ "\nRace - " + race.name 
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
		+ "\nCharisma = " + std::to_string(charisma) + "\n";
	return text;
}

std::string Character::GetInventoryStats()
{
	std::string text = "";
	for (const auto& item : inventory)
	{
		text += item -> GetName() + ", ";
	}
	return text;
}

void Character::AddItemToInventory(std::shared_ptr<Item> item)
{
	if (inventory.size() < max_inventory_size) 
	{
		inventory.push_back(std::move(item));
	}
}

void Character::DeleteItemFromInventory(const std::string& itemName)
{
	for (auto it = inventory.begin(); it != inventory.end(); it++)
	{
		if ((*it)->GetName() == itemName)
		{
			inventory.erase(it);
			break;
		}
	}
}

std::shared_ptr<Item> Character::GetItemFromInventory(const std::string& itemName) 
{
	for (auto& item : inventory)
	{
		if (item -> GetName() == itemName)
		{
			return item;
		}
	}

	return nullptr;
}

int Character::CountItemInInventory(const std::string& itemName)
{
	return static_cast<int>
		(std::count_if(inventory.begin(), inventory.end(),
		[&itemName](const std::shared_ptr<Item>& item) 
		{ return item->GetName() == itemName; }));
}