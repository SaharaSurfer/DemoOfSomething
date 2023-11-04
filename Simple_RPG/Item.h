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
	void SetName(std::string new_name);

	std::string GetDescription();
	void SetDescription(std::string new_descr);

	std::string GetEffect();
	void SetEffect(std::string new_effect);


	virtual ~Item();
};