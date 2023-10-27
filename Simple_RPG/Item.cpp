#include "Item.h"

void Item::LoadData(std::string item_name)
{
	json items = interface.LoadJSON("JsonFile\\Items.json");
	name = items[item_name]["name"];
	description = items[item_name]["description"];
	effect = items[item_name]["effect"];
}

Item::Item() {}

std::string Item::GetName()
{
	return name;
}

std::string Item::GetDescription()
{
	return description;
}

std::string Item::GetEffect()
{
	return effect;
}