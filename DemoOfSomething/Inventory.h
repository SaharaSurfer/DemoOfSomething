#pragma once
#include "Item.h"

class Inventory
{
private:
	std::vector<std::shared_ptr<Item>> inventory;
	static const int max_inventory_size = 10;

public:
	void AddItem(std::shared_ptr<Item> item);
	void DeleteItem(const std::string& itemName);
	std::shared_ptr<Item> GetItem(const std::string& itemName);
	int CountItem(const std::string& itemName);

	std::vector<std::shared_ptr<Item>> GetInventory();
};