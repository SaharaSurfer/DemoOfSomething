#include "../header/Item.h"

void Item::LoadData(const std::string& item_name)
{
	json items = interface.LoadJSON("JsonFiles\\Items.json");
	name = items[item_name]["name"];
	description = items[item_name]["description"];
	effect = items[item_name]["effect"];
}

Item::Item() {}

std::string Item::GetName()
{
	return name;
}

void Item::SetName(const std::string& new_name)
{
	name = new_name;
}

std::string Item::GetDescription()
{
	return description;
}

void Item::SetDescription(const std::string& new_descr)
{
	description = new_descr;
}

std::string Item::GetEffect()
{
	return effect;
}

void Item::SetEffect(const std::string& new_effect)
{
	effect = new_effect;
}

Item::~Item() {};