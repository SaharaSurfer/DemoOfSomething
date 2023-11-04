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

	std::vector<Item*> equipment;
	std::vector<Item*> inventory;
	std::vector<Ability> abilities;

public:
	std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects, negative_effects;

	Character();
	Character(Race r, GameClass gc);

	std::string GetName();
	void SetName(std::string new_name);

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

	void AddItemToInventory(Item* object);
	void DeleteItemFromInventory(std::string name);
	Item* GetItemFromInventory(std::string name);
	int CountItemInInventory(std::string name);
};