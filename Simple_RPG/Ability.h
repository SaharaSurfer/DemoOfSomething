#pragma once
#include <string>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

struct Ability
{
	std::string name = "";
	std::string description = "";
	std::string resource = "";

	int resource_cost = 0;
	int damage = 0, health = 0, mana = 0;
	int periodic_damage = 0;
	int damage_boost = 0;
	int defense_boost = 0;
	int turns = 0;
	int enemy = 0;

	Ability();
	Ability(json info);

	void ShowInfo();
};