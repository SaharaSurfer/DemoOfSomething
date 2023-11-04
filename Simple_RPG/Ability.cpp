#include "Ability.h"

Ability::Ability() {}

Ability::Ability(json info)
{
	name = info["name"];
	description = info["description"];

	if (info.contains("mana_cost")) { resource = "mana"; resource_cost = info["mana_cost"]; }
	if (info.contains("energy_cost")) { resource = "energy", resource_cost = info["energy_cost"]; }
	if (info.contains("damage")) { damage = info["damage"]; }
	if (info.contains("health")) { health = info["health"]; }
	if (info.contains("mana")) { mana = info["mana"]; }
	if (info.contains("periodic_damage")) { periodic_damage = info["periodic_damage"]; }
	if (info.contains("damage_boost")) { damage_boost = info["damage_boost"]; }
	if (info.contains("defense_boost")) { defense_boost = info["defense_boost"]; }
	if (info.contains("turns")) { turns = info["turns"]; }
	if (info.contains("enemy")) { enemy = info["enemy"]; }
}

void Ability::ShowInfo()
{
	std::string text = name + " (" + description + ")\n";
	if (resource != "") 
	{
		text += "costs " + std::to_string(resource_cost) + " " + resource + "\n";
	}

	if (damage != 0) { text += "deals " + std::to_string(damage) + " damage\n"; }
	if (health != 0) { text += "restores " + std::to_string(health) + " health\n"; }
	if (mana != 0) { text += "restores " + std::to_string(mana) + " mana\n"; }
	if (periodic_damage != 0) { text += "deals " + std::to_string(periodic_damage) + " periodic damage\n"; }
	if (damage_boost != 0) { text += "Increases damage by " + std::to_string(damage_boost) + "%\n"; }
	if (defense_boost != 0) { text += "Increases defense by " + std::to_string(defense_boost) + "%\n"; }
	if (turns != 0) { text += "Lasts " + std::to_string(turns) + " turns\n"; }
	if (enemy != 0) { text += "Affects to " + std::to_string(enemy) + " enemies\n"; }

	std::cout << text << "\n";
}