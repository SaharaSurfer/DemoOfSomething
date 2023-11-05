#pragma once
#include <string>
#include "Interface.h"

class Item
{
protected:
	Interface interface;

	std::string name = "";
	std::string description = "";
	std::string effect = "";

	void LoadData(const std::string& item_name);
public:
	Item();

	std::string GetName();
	void SetName(const std::string& new_name);

	std::string GetDescription();
	void SetDescription(const std::string& new_descr);

	std::string GetEffect();
	void SetEffect(const std::string& new_effect);

	virtual ~Item();
};