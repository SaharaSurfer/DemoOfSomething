#pragma once
#include <iostream>
#include <vector>
#include "Interface.h"
#include "Item.h"
#include "Ability.h"
#include "Race.h"
#include "GameClass.h"

class Character
{
protected:
	Race race;
	GameClass gameclass;

	std::string name;
	int experience = 0;
	int level = 1;
	int health = 100, max_health = 100;
	int mana = 100, max_mana = 100;
	int energy = 100, max_energy = 100;
	int defense = 10, attack = 15;
	int strength = 10, dexterity = 10;
	int intelligence = 10, wisdom = 10, charisma = 10;

	std::vector<std::shared_ptr<Item>> equipment;
	std::vector<std::shared_ptr<Item>> inventory;
	static const int max_inventory_size = 10;
	std::vector<Ability> abilities;

public:
	std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects, negative_effects;

	Character();
	Character(const Race& r, const GameClass& gc);

	void LoadAbilitiesFromJSON();

	std::string GetRaceName();
	std::string GetClassName();
	std::string GetName();
	void SetName(const std::string& new_name);

	void GainExp(int exp);

	int GetHealthPercent();
	int GetMaxHealth();
	int GetHealth();
	void IncreaseHealth(int shift);
	void IncreaseMana(int shift);
	void IncreaseEnergy(int shift);

	int GetAttack();
	int GetMana();
	int GetEnergy();

	int GetStatBonus(std::string stat_name);

	std::string GetCharacterStats();
	std::string GetInventoryStats();

	void AddItemToInventory(std::shared_ptr<Item> item);
	void DeleteItemFromInventory(const std::string& itemName);
	std::shared_ptr<Item> GetItemFromInventory(const std::string& itemName);
	int CountItemInInventory(const std::string& itemName);
};