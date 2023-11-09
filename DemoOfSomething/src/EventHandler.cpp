#include "../header/EventHandler.h"

int EventHandler::HandleEvent(json node, Character& player)
{
	static const std::unordered_map<std::string, std::function<int(EventHandler&, json, Character&)>> eventHandlers =
	{
		{"ReceiveItems", &EventHandler::ReceiveItems},
		{"ProcessLockpicking", &EventHandler::ProcessLockpicking},
		{"UseKeyToOpen", &EventHandler::UseKeyToOpen},
		{"ProcessDialog", &EventHandler::ProcessDialog},
		{"ProcessSurpriseAttack", &EventHandler::ProcessSurpriseAttack},
		{"ProcessFight", &EventHandler::ProcessFight}
	};

	const std::string event_type = node["event"];
	if (eventHandlers.find(event_type) != eventHandlers.end()) {
		return eventHandlers.at(event_type)(*this, node, player);
	}
	
	return 0;
}

int EventHandler::ReceiveItems(json node, Character& player)
{
	static const std::unordered_map<std::string, std::function<std::shared_ptr<Item>()>> item_сreators = 
	{
		{"Lockpick", []() { return std::make_shared<Lockpick>(); }},
		{"Key", []() { return std::make_shared<Key>(); }},
	};

	for (int i = 0; i < node["number_of_items"]; i++) 
	{
		std::string item_name = std::string(node["item_name"]);
		auto item_creator = item_сreators.find(item_name);
		if (item_creator != item_сreators.end()) 
		{
			std::shared_ptr<Item> item = item_creator -> second();
			if (item_name == "Key") 
			{
				auto key = std::dynamic_pointer_cast<Key>(item);
				key -> SetName(std::string(node["key_name"]));
			}
			player.inventory.AddItem(std::move(item));
		}
	}

	return 0;
}

int EventHandler::ProcessLockpicking(json node, Character& player)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, 2);

	json failure_text = node["fail"];

	if (node["interactive_object_name"] == "Lock")
	{
		Lock lock = Lock();
		lock.SetLockLevel(node["security_level"]);

		while (player.inventory.CountItem("Lockpick") > 0)
		{
			std::shared_ptr<Item> item = player.inventory.GetItem("Lockpick");
			std::shared_ptr<Lockpick> lockpick = std::dynamic_pointer_cast<Lockpick>(item);

			bool is_open = lockpick -> UnlockObject(lock, player);
			if (is_open)
			{
				interface.RenderText(std::string(node["success"]) + "\n");
				return 1;
			}

			interface.RenderText(std::string(failure_text[distribution(gen)]) + "\n");
			player.inventory.DeleteItem("Lockpick");

			int lockpick_num = player.inventory.CountItem("Lockpick");
			std::string text =
				"<You have " +
				std::to_string(lockpick_num) +
				" of lock picks left>\n";
			interface.RenderText(text);

			if (lockpick_num == 0) { return 0; }
			return 2;
		}
	}
	interface.RenderText(std::string(node["none"]));
	return 0;
}

int EventHandler::UseKeyToOpen(json node, Character& player)
{
	if (node["interactive_object_name"] == "Lock")
	{
		Lock lock = Lock();
		lock.SetKeyName(std::string(node["key_needed"]));

		std::shared_ptr<Item> item = player.inventory.GetItem(std::string(node["key_needed"]));
		if (auto key = std::dynamic_pointer_cast<Key>(item))
		{
			key -> UnlockObject(lock);
			interface.RenderText(std::string(node["success"]));
		}
		else
		{
			interface.RenderText(std::string(node["none"]));
			return 1;
		}
	}

	return 0;
}

int EventHandler::ProcessDialog(json node, Character& player)
{
	json dialog = interface.LoadJSON(std::string(node["dialog_name"]));

	std::deque<json> current_options;

	//Filling in initial options
	for (const auto& option : dialog)
	{
		current_options.push_back(option);
	}

	while (!current_options.empty())
	{
		//Bringing up the possible player choices
		for (size_t i = 0; i < current_options.size(); i++)
		{
			std::string text = std::to_string(i + 1) + ") " + std::string(current_options[i]["choice_name"]) + "\n";
			std::cout << text;
		}

		//Getting the player's solution
		size_t choice = interface.CollectPlayerChoice(current_options.size());
		
		const json& chosen_node = current_options[choice];
		interface.RenderText(std::string(chosen_node["hero_line"]) + "\n");
		interface.RenderText(std::string(chosen_node["npc_line"]) + "\n");

		if (chosen_node["is_end"] == 1)
		{
			current_options.clear();
		}

		//Removing already used branch
		if (!current_options.empty())
		{
			current_options.erase(current_options.begin() + choice - 1);
		}

		//Recording new branches if any
		if (chosen_node.contains("options"))
		{
			for (const auto& node : chosen_node["options"])
			{
				current_options.push_back(node);
			}
		}
	}

	return 0;
}

int EventHandler::ProcessSurpriseAttack(json node, Character& player)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 20);

	if (distribution(gen) + player.characteristics.GetStatBonus(node["stat_check"]) < node["complexity"])
	{
		interface.RenderText(node["fail"]);
		return 1;
	}
	else
	{
		interface.RenderText(node["success"]);
		return 0;
	}
}

int EventHandler::ProcessFight(json node, Character& player)
{
	BattleHandler battle(node, player);
	int is_player_lost = battle.HandleFight();

	return is_player_lost;
}
