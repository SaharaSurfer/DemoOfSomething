#include "CharacterCharacteristics.h"

CharacterCharacteristics::CharacterCharacteristics() {}

CharacterCharacteristics::CharacterCharacteristics(const Race& r, const GameClass& gc) : race(r), gameclass(gc)
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
}

CharacterCharacteristics::CharacterCharacteristics(const Race& r, const GameClass& gc, int lvl) : race(r), gameclass(gc), level(lvl)
{
	CharacterCharacteristics(r, gc);
}

std::string CharacterCharacteristics::GetRaceName()
{
	return race.name;
}

std::string CharacterCharacteristics::GetClassName()
{
	return gameclass.name;
}

int CharacterCharacteristics::GetSTR()
{
	return strength;
}

int CharacterCharacteristics::GetDEX()
{
	return dexterity;
}

int CharacterCharacteristics::GetINT()
{
	return intelligence;
}

int CharacterCharacteristics::GetWIS()
{
	return wisdom;
}

int CharacterCharacteristics::GetCHA()
{
	return charisma;
}

int CharacterCharacteristics::GetHealthPercent()
{
	return (int)((float)health / max_health * 100);
}

int CharacterCharacteristics::GetMaxHealth()
{
	return max_health;
}

int CharacterCharacteristics::GetHealth()
{
	return health;
}

int CharacterCharacteristics::GetMaxMana()
{
	return max_mana;
}

int CharacterCharacteristics::GetMana()
{
	return mana;
}

int CharacterCharacteristics::GetMaxEnergy()
{
	return max_energy;
}

int CharacterCharacteristics::GetEnergy()
{
	return energy;
}

void CharacterCharacteristics::IncreaseHealth(int shift, std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects)
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

void CharacterCharacteristics::IncreaseMana(int shift)
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

void CharacterCharacteristics::IncreaseEnergy(int shift)
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


int CharacterCharacteristics::GetAttack()
{
	return attack;
}

int CharacterCharacteristics::GetDefense()
{
	return defense;
}

int CharacterCharacteristics::GetLevel()
{
	return level;
}

void CharacterCharacteristics::IncrementLevel()
{
	level++;
}

int CharacterCharacteristics::GetStatBonus(std::string stat_name)
{
	static std::map<int, int> bonuses
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
