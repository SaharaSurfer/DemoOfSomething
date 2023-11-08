#include "../header/Inventory.h"

void Inventory::AddItem(std::shared_ptr<Item> item)
{
	if (inventory.size() < max_inventory_size)
	{
		inventory.push_back(std::move(item));
	}
}

void Inventory::DeleteItem(const std::string& itemName)
{
	for (auto it = inventory.begin(); it != inventory.end(); it++)
	{
		if ((*it)->GetName() == itemName)
		{
			inventory.erase(it);
			break;
		}
	}
}

std::shared_ptr<Item> Inventory::GetItem(const std::string& itemName)
{
	for (auto& item : inventory)
	{
		if (item->GetName() == itemName)
		{
			return item;
		}
	}

	return nullptr;
}

int Inventory::CountItem(const std::string& itemName)
{
	return static_cast<int>
		(std::count_if(inventory.begin(), inventory.end(),
			[&itemName](const std::shared_ptr<Item>& item)
			{ return item->GetName() == itemName; }));
}

std::vector<std::shared_ptr<Item>> Inventory::GetInventory()
{
	return inventory;
}
