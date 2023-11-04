#pragma once
#include <string>
#include "json.hpp"

struct Race
{
	std::string name;
	int strengthBonus;
	int dexterityBonus;
	int intelligenceBonus;
	int wisdomBonus;
	int charismaBonus;
};

static void from_json(const json& j, Race& r)
{
	j.at("name").get_to(r.name);
	j.at("strengthBonus").get_to(r.strengthBonus);
	j.at("dexterityBonus").get_to(r.dexterityBonus);
	j.at("intelligenceBonus").get_to(r.intelligenceBonus);
	j.at("wisdomBonus").get_to(r.wisdomBonus);
	j.at("charismaBonus").get_to(r.charismaBonus);
}