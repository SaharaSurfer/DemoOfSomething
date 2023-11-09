#include "../header/Character.h"

Character::Character() {}

Character::Character(const Race& r, const GameClass& gc)
{
	characteristics = CharacterCharacteristics(r, gc);
	LoadAbilitiesFromJSON();
}

Character::Character(const Race& r, const GameClass& gc, int lvl)
{
	characteristics = CharacterCharacteristics(r, gc, lvl);
	LoadAbilitiesFromJSON();
}

void Character::LoadAbilitiesFromJSON()
{
	Interface interface;
	const json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");

	std::string class_name = characteristics.GetClassName();
	for (size_t i = 0; i < list_of_abilities[class_name].size() and i < characteristics.GetLevel(); i++)
	{
		for (const auto& node : list_of_abilities[class_name][i])
		{
			abilities.push_back(Ability(node));
		}
	}
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
	
	int level = characteristics.GetLevel();
	if (experience >= exp_progression[level])
	{
		experience %= exp_progression[level];
		characteristics.IncrementLevel();
		characteristics.AddClassBonus();

		Interface interface;
		const json list_of_abilities = interface.LoadJSON("JsonFiles\\Abilities.json");
		for (const auto& node : list_of_abilities[characteristics.GetClassName()][level])
		{
			abilities.push_back(Ability(node));
		}
	}
}

bool Character::IsDead()
{
	return characteristics.GetHealth() == 0;
}

std::vector<Ability> Character::GetAbilities()
{
	return abilities;
}

std::string Character::GetInventoryStats()
{
	std::string text = "";
	for (const auto& item : inventory.GetInventory())
	{
		text += item -> GetName() + ", ";
	}
	return text;
}

std::string Character::GetResourceInfo()
{
	const int health_pallets = 10;
	const int health_percent_padding = 4;

	int numPallets = (characteristics.GetHealthPercent() + health_percent_padding) / health_pallets;
	std::string healthBar = std::string(numPallets, '#') + std::string(health_pallets - numPallets, '*');
	std::string text = name + "(" + characteristics.GetRaceName() + " " + characteristics.GetClassName() + ")"
		+ ": " + healthBar + " " + std::to_string(characteristics.GetHealth())
		+ "/" + std::to_string(characteristics.GetMaxHealth()) + " " 
		+ std::to_string(characteristics.GetMana()) + " mana "
		+ std::to_string(characteristics.GetEnergy()) + " energy\n";

	return text;
}

std::string Character::GetCharacterStats()
{
	std::string text =
		"Level - " + std::to_string(characteristics.GetLevel())
		+ "\nRace - " + characteristics.GetRaceName()
		+ "\nClass - " + characteristics.GetClassName()
		+ "\n-------------------------------"
		+ "\nHealth = " + std::to_string(characteristics.GetHealth())
		+ "\nMana = " + std::to_string(characteristics.GetMana())
		+ "\nDefense = " + std::to_string(characteristics.GetDefense())
		+ "\nAttack = " + std::to_string(characteristics.GetAttack())
		+ "\n-------------------------------"
		+ "\nStrength = " + std::to_string(characteristics.GetSTR())
		+ "\nDexterity = " + std::to_string(characteristics.GetDEX())
		+ "\nIntelligence = " + std::to_string(characteristics.GetINT())
		+ "\nWisdom = " + std::to_string(characteristics.GetWIS())
		+ "\nCharisma = " + std::to_string(characteristics.GetCHA()) + "\n";

	return text;
}