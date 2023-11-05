#pragma once
#include <iostream>
#include <vector>
#include "Interface.h"
#include "Item.h"
#include "Ability.h"
#include "Race.h"
#include "GameClass.h"
#include "CharacterCharacteristics.h"

class Character
{
protected:
	std::string name;
	int experience = 0;

	std::vector<std::shared_ptr<Item>> equipment;
	std::vector<std::shared_ptr<Item>> inventory;
	static const int max_inventory_size = 10;
	std::vector<Ability> abilities;

public:
	CharacterCharacteristics characteristics;

	std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects, negative_effects;

	Character();
	Character(const Race& r, const GameClass& gc);
	Character(const Race& r, const GameClass& gc, int lvl);

	void LoadAbilitiesFromJSON();

	std::string GetName();
	void SetName(const std::string& new_name);

	void GainExp(int exp);

	bool IsDead();

	std::vector<Ability> GetAbilities();

	std::string GetInventoryStats();

	void AddItemToInventory(std::shared_ptr<Item> item);
	void DeleteItemFromInventory(const std::string& itemName);
	std::shared_ptr<Item> GetItemFromInventory(const std::string& itemName);
	int CountItemInInventory(const std::string& itemName);
	
	std::string GetResourceInfo();
	std::string GetCharacterStats();
};