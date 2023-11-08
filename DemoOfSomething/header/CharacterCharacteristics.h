#pragma once
#include <string>
#include <map>
#include <vector>
#include "Race.h"
#include "GameClass.h"

class CharacterCharacteristics
{
private:
	Race race;
	GameClass gameclass;

	int health = 100, max_health = 100;
	int mana = 100, max_mana = 100;
	int energy = 100, max_energy = 100;
	int defense = 10, attack = 15;
	int strength = 10, dexterity = 10;
	int intelligence = 10, wisdom = 10, charisma = 10;

	int level = 1;

public:
	CharacterCharacteristics();
	CharacterCharacteristics(const Race& r, const GameClass& gc);
	CharacterCharacteristics(const Race& r, const GameClass& gc, int lvl);

	void AddStatBonus();

	std::string GetRaceName();
	std::string GetClassName();

	int GetSTR();
	int GetDEX();
	int GetINT();
	int GetWIS();
	int GetCHA();

	int GetHealthPercent();
	int GetMaxHealth();
	int GetHealth();

	int GetMaxMana();
	int GetMana();

	int GetMaxEnergy();
	int GetEnergy();

	void IncreaseHealth(int shift, std::vector<std::pair<std::string, std::pair<int, int>>> positive_effects);
	void IncreaseMana(int shift);
	void IncreaseEnergy(int shift);

	int GetAttack();
	int GetDefense();

	int GetLevel();
	void IncrementLevel();

	int GetStatBonus(std::string stat_name);
};