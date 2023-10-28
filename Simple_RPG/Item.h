#pragma once
#include <string>
#include "Interface.h"

class Item
{
protected:
	Interface interface;

	std::string name;
	std::string description;
	std::string effect;

	void LoadData(std::string item_name);
public:
	Item();

	std::string GetName();
	std::string GetDescription();
	std::string GetEffect();

	virtual ~Item();
};