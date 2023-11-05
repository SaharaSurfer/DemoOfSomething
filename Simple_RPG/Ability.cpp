#include "Ability.h"

Ability::Ability() {}

Ability::Ability(json info)
{
	name = info["name"];
	description = info["description"];

	ParseAbilityDetails(info);
}

void Ability::AssignValue(const std::string& key, int& value, const json& info) 
{
	if (info.contains(key)) 
	{
		value = info[key];
	}
}

void Ability::AssignValue(const std::string& key, const std::string& res, int& res_cost, const json& info) 
{
	if (info.contains(key)) 
	{
		resource = res;
		res_cost = info[key];
	}
}

void Ability::ParseAbilityDetails(const json& info) {
	AssignValue("mana_cost", "mana", resource_cost, info);
	AssignValue("energy_cost", "energy", resource_cost, info);
	AssignValue("damage", damage, info);
	AssignValue("health", health, info);
	AssignValue("mana", mana, info);
	AssignValue("periodic_damage", periodic_damage, info);
	AssignValue("damage_boost", damage_boost, info);
	AssignValue("defense_boost", defense_boost, info);
	AssignValue("turns", turns, info);
	AssignValue("enemy", enemy, info);
}

void Ability::AddAbilityDetail(std::string& text, const std::string& action, int value, const std::string& attribute) 
{
	if (value != 0) 
	{
		text += action + " " + std::to_string(value) + attribute + "\n";
	}
}

void Ability::ShowInfo()
{
	std::string text = name + " (" + description + ")\n";
	if (!resource.empty()) 
	{
		text += "Costs " + std::to_string(resource_cost) + " " + resource + "\n";
	}

	AddAbilityDetail(text, "Deals", damage, " damage");
	AddAbilityDetail(text, "Restores", health, " health");
	AddAbilityDetail(text, "Restores", mana, " mana");
	AddAbilityDetail(text, "Deals", periodic_damage, " periodic damage");
	AddAbilityDetail(text, "Increases damage by", damage_boost, "%");
	AddAbilityDetail(text, "Increases defense by", defense_boost, "%");
	AddAbilityDetail(text, "Lasts", turns, " turns");
	AddAbilityDetail(text, "Affects to", enemy, " enemies");

	std::cout << text << "\n";
}