#include "../header/ReceiveItemsEvent.h"

const std::unordered_map<std::string, std::function<std::shared_ptr<Item>()>> ReceiveItemsEvent::item_creators =
{
	{"Lockpick", []() { return std::make_shared<Lockpick>(); }},
	{"Key", []() { return std::make_shared<Key>(); }},
};

int ReceiveItemsEvent::Execute(json node, Character& player, Interface& interface)
{
	for (int i = 0; i < node["number_of_items"]; i++)
	{
		std::string item_name = std::string(node["item_name"]);
		auto item_creator = item_creators.find(item_name);
		if (item_creator != item_creators.end())
		{
			std::shared_ptr<Item> item = item_creator->second();
			if (item_name == "Key")
			{
				auto key = std::dynamic_pointer_cast<Key>(item);
				key->SetName(std::string(node["key_name"]));
			}
			player.inventory.AddItem(std::move(item));
		}
	}

	return 0;
}