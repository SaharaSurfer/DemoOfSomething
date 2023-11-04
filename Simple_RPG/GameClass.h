#pragma once
#include <string>
#include "json.hpp"

struct GameClass
{
	std::string name;
	int healthBonus;
	int manaBonus;
	int defenseBonus;
	int attackBonus;
};

static void from_json(const json& j, GameClass& gc)
{
	j.at("name").get_to(gc.name);
	j.at("healthBonus").get_to(gc.healthBonus);
	j.at("manaBonus").get_to(gc.manaBonus);
	j.at("defenseBonus").get_to(gc.defenseBonus);
	j.at("attackBonus").get_to(gc.attackBonus);
}