#pragma once
#include <iostream>
#include <vector>
#include "Item.h"
#include "Ability.h"
#include "Race.h"
#include "GameClass.h"

class Character
{
protected:
	Race race;
	GameClass gameclass;

	int level = 1;
	int health = 100, mana = 100;
	int defense = 10, attack = 15;
	int strength = 10, dexterity = 10;
	int intelligence = 10, wisdom = 10, charisma = 10;

	std::vector<Item*> equipment;
	std::vector<Item*> inventory;
	std::vector<Ability> abilities;

public:
	Character();
	Character(Race r, GameClass gc);

	int GetDexBonus();

	std::string GetCharacterStats();
	std::string GetInventoryStats();

	void AddItemToInventory(Item* object);
	void DeleteItemFromInventory(std::string name);
	Item* GetItemFromInventory(std::string name);
	int CountItemInInventory(std::string name);
};