#pragma once
#include <iostream>
#include <vector>
#include "Interface.h"
#include "Item.h"
#include "Ability.h"
#include "Race.h"
#include "GameClass.h"
#include "CharacterCharacteristics.h"
#include "Inventory.h"

class Character
{
private:
	std::string name;
	int experience = 0;
	std::vector<Ability> abilities;

	void LoadAbilitiesFromJSON();

public:
	CharacterCharacteristics characteristics;
	Inventory inventory;

	std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects, negative_effects;

	Character();
	Character(const Race& r, const GameClass& gc);
	Character(const Race& r, const GameClass& gc, int lvl);

	std::string GetName();
	void SetName(const std::string& new_name);

	std::vector<Ability> GetAbilities();

	void GainExp(int exp);
	bool IsDead();

	std::string GetInventoryStats();
	std::string GetResourceInfo();
	std::string GetCharacterStats();
};