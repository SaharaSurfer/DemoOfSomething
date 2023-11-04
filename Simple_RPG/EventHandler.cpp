#include "EventHandler.h"

int EventHandler::HandleEvent(json node, Player& player)
{
	std::string event_type = node["event"];

	int is_over = 0;
	if (event_type == "ReceiveItems")
	{
		is_over = ReceiveItems(node, player);
	}
	else if (event_type == "ProcessLockpicking")
	{
		is_over = ProcessLockpicking(node, player);
	}
	else if (event_type == "UseKeyToOpen")
	{
		is_over = UseKeyToOpen(node, player);
	}
	else if (event_type == "ProcessDialog")
	{
		is_over = ProcessDialog(node);
	}
	else if (event_type == "ProcessSurpriseAttack")
	{
		is_over = ProcessSurpriseAttack(node, player);
	}
	else if (event_type == "ProcessFight")
	{
		is_over = ProcessFight(node, player);
	}

	return is_over;
}

int EventHandler::ReceiveItems(json node, Player& player)
{
	for (int i = 0; i < node["number_of_items"]; i++)
	{
		Item* item = new Item;
		if (node["item_name"] == std::string("Lockpick"))
		{
			item = new Lockpick();
		}
		else if (node["item_name"] == std::string("Key"))
		{
			item = new Key();
			item->SetName(std::string(node["key_name"]));
		}
		player.AddItemToInventory(item);
	}

	return 0;
}

int EventHandler::ProcessLockpicking(json node, Player& player)
{
	srand(time(0));
	json failure_text = node["fail"];

	if (node["interactive_object_name"] == "Lock")
	{
		Lock lock = Lock();
		lock.SetLockLevel(node["security_level"]);

		while (player.GetInventoryStats().find("Lockpick") != std::string::npos)
		{
			Item* item = player.GetItemFromInventory("Lockpick");
			Lockpick* lockpick = dynamic_cast<Lockpick*>(item);

			bool is_open = lockpick->UnlockObject(lock, player);
			if (is_open)
			{
				interface.RenderText(std::string(node["success"]) + "\n");
				return 1;
			}

			interface.RenderText(std::string(failure_text[rand() % 3]) + "\n");
			player.DeleteItemFromInventory("Lockpick");

			int lockpick_num = player.CountItemInInventory("Lockpick");
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

int EventHandler::UseKeyToOpen(json node, Player& player)
{
	if (node["interactive_object_name"] == "Lock")
	{
		// Можно передавать замок, а не создавать его здесь
		Lock lock = Lock();
		lock.SetKeyName(std::string(node["key_needed"]));

		Item* item = player.GetItemFromInventory(std::string(node["key_needed"]));
		if (item != nullptr)
		{
			Key* key = dynamic_cast<Key*>(item);
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

int EventHandler::ProcessDialog(json node)
{
	json dialog = interface.LoadJSON(std::string(node["dialog_name"]));

	std::vector<json> current_options;

	//Filling in initial options
	for (auto& option : dialog)
	{
		current_options.push_back(option);
	}

	while (current_options.size() > 0)
	{
		//Bringing up the possible player choices
		for (int i = 0; i < current_options.size(); i++)
		{
			std::string text = std::to_string(i + 1) + ") " + std::string(current_options[i]["choice_name"]) + "\n";
			interface.RenderText(text);
		}

		//Getting the player's solution
		std::string player_choice;
		std::getline(std::cin, player_choice);
		player_choice = player_choice.substr(0, 1);

		json chosen_node = current_options[std::stoi(player_choice) - 1];

		interface.RenderText(std::string(chosen_node["hero_line"]) + "\n");
		interface.RenderText(std::string(chosen_node["npc_line"]) + "\n");

		if (chosen_node["is_end"] == 1)
		{
			current_options.clear();
		}

		//Removing already used branch
		if (current_options.size() > 0)
		{
			current_options.erase(current_options.begin() + std::stoi(player_choice) - 1);
		}

		//Recording new branches if any
		if (chosen_node.contains("options"))
		{
			for (auto& node : chosen_node["options"])
			{
				current_options.push_back(node);
			}
		}
	}

	return 0;
}

int EventHandler::ProcessSurpriseAttack(json node, Player& player)
{
	srand(time(0));

	if (rand() % 20 + 1 + player.GetStatBonus(node["stat_check"]) < node["complexity"])
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

int EventHandler::ProcessFight(json node, Player& player)
{
	std::vector<NPC> enemies = CreateNPCs(node);

	srand(time(0));
	int player_initiative = rand() % 20 + 1;
	
	std::vector<int> enemy_initiatives;
	for (size_t i = 0; i < enemies.size(); i++)
	{
		int enemy_initiative = rand() % 20 + 1;
		enemy_initiatives.push_back(enemy_initiative);
	}
	
	std::vector<int> all_initiatives = enemy_initiatives;
	all_initiatives.push_back(player_initiative);
	std::sort(all_initiatives.rbegin(), all_initiatives.rend());

	while (std::any_of(enemies.cbegin(), enemies.cend(), [](NPC npc) { return !npc.IsDead(); }))
	{
		for (int init_ind = 0; init_ind < all_initiatives.size(); init_ind++)
		{
			if (all_initiatives[init_ind] == player_initiative)
			{
				OutputCurrentFightState(player, enemies);
				PlayerTurn(player, enemies);
			}
			else
			{
				int enemy_ind = std::find(enemy_initiatives.rbegin(), enemy_initiatives.rend(), 
					all_initiatives[init_ind]) - enemy_initiatives.rbegin();
				if (enemies[enemy_ind].IsDead()) { continue; }
				NPCTurn(player, enemies[enemy_ind]);
			}
		}
	}

	return 1;
}

std::vector<NPC> EventHandler::CreateNPCs(json node)
{
	json classes = interface.LoadJSON("JsonFiles\\Class.json");
	json races = interface.LoadJSON("JsonFiles\\Race.json");

	std::vector<NPC> enemies;
	for (int i = 0; i < node["num_of_enemy"]; i++)
	{
		GameClass gc;
		classes[0][std::string(node["classes_of_enemy"][i])].get_to(gc);

		Race r;
		races[0][std::string(node["races_of_enemy"][i])].get_to(r);

		NPC enemy = NPC(r, gc, node["levels"][i]);
		enemies.push_back(enemy);
	}

	return enemies;
}

void EventHandler::OutputCurrentFightState(Player& player, std::vector<NPC> enemies)
{
	std::cout << std::string(50, '-') << std::endl;
	std::cout << "Player Turn\n";
	std::cout << std::string(50, '-') << std::endl;
	int num_of_pallets = (player.GetHealthPercent() + 4) / 10;
	std::cout << "Player Health: " << 
		std::string(num_of_pallets, '#') + std::string(10 - num_of_pallets, '*') + " "
		<< player.GetHealth() << "/" << player.GetMaxHealth() 
		<< " " << player.GetMana() << " mana " << player.GetEnergy() << " energy\n";
	std::cout << std::string(50, '-') << std::endl;

	for (size_t i = 0; i < enemies.size(); i++)
	{
		int num_of_pallets = (enemies[i].GetHealthPercent() + 4) / 10;
		std::cout << i + 1 << ") " << enemies[i].GetName() 
			<< "(" << enemies[i].GetRaceName() << " " << enemies[i].GetClassName() << ")" 
			<< ": " << std::string(num_of_pallets, '#') + std::string(10 - num_of_pallets, '*') + " "
			<< enemies[i].GetHealth() << "/" << enemies[i].GetMaxHealth() 
			<< " " << enemies[i].GetMana() << " mana " << enemies[i].GetEnergy() << " energy\n";
	}

	std::cout << std::string(50, '-') << std::endl;
}

void EventHandler::PlayerTurn(Player& player, std::vector<NPC>& enemies)
{
	for (std::pair<std::string, std::pair<int, int>> effect : player.positive_effects)
	{
		if (effect.second.second > 0) { effect.second.second -= 1; }
	}

	for (std::pair<std::string, std::pair<int, int>> effect : player.negative_effects)
	{
		if (effect.first == "periodic_damage" and effect.second.second > 0)
		{
			player.IncreaseHealth(-effect.second.first);
			effect.second.second -= 1;
		}
	}

	std::cout << "1) Attack with weapon in hand\n";
	std::cout << "2) Use ability\n";
	std::cout << "3) Use item\n";
	std::cout << "Enter your choice: ";

	std::string player_choice;
	std::getline(std::cin, player_choice);
	player_choice = player_choice.substr(0, 1);

	if (player_choice == "1")
	{
		std::cout << "Enter target: ";
		std::string target_choice;
		std::getline(std::cin, target_choice);
		target_choice = target_choice.substr(0, 1);

		float damage = player.GetAttack();
		for (std::pair<std::string, std::pair<int, int>> effect : player.positive_effects)
		{
			if (effect.first == "damage_boost" and effect.second.second > 0)
			{
				damage *= (1 + effect.second.first / 100);
			}
		}
		enemies[std::stoi(target_choice) - 1].IncreaseHealth(-(int)damage);
	}
	else if (player_choice == "2")
	{
		int ability_ind = 0;
		for (Ability ability : player.GetAbilities())
		{
			ability_ind++;

			std::cout << ability_ind << ") ";
			ability.ShowInfo();
		}

		std::cout << "Enter ability choice: ";
		std::string ability_choice;
		std::getline(std::cin, ability_choice);
		ability_choice = ability_choice.substr(0, 1);

		Ability chosen_ability = player.GetAbilities()[std::stoi(ability_choice) - 1];
		if (chosen_ability.resource == "mana") { player.IncreaseMana(-chosen_ability.resource_cost); }
		if (chosen_ability.resource == "energy") { player.IncreaseEnergy(-chosen_ability.resource_cost); }
		if (chosen_ability.damage != 0)
		{
			std::cout << "Enter target: ";
			std::string target_choice;
			std::getline(std::cin, target_choice);
			target_choice = target_choice.substr(0, 1);

			float damage = chosen_ability.damage;
			for (std::pair<std::string, std::pair<int, int>> effect : player.positive_effects)
			{
				if (effect.first == "damage_boost" and effect.second.second > 0)
				{
					damage *= (1 + effect.second.first / 100);
				}
			}

			enemies[std::stoi(target_choice) - 1].IncreaseHealth(-(int)damage);

			if (chosen_ability.periodic_damage != 0)
			{
				enemies[std::stoi(target_choice) - 1].negative_effects.push_back(
					std::pair<std::string, std::pair<int, int>>("periodic_damage", std::pair<int, int>(chosen_ability.periodic_damage, chosen_ability.turns)));
			}
			
			if (chosen_ability.enemy != 0)
			{
				int enemy_ind = -1, damaged_enemies = 0;
				while (damaged_enemies < (chosen_ability.enemy - 1) and (enemy_ind + 1) < enemies.size())
				{
					enemy_ind++;
					if (enemy_ind == std::stoi(target_choice) - 1) { continue; }
					enemies[enemy_ind].IncreaseHealth(-(int)damage);
					damaged_enemies++;
				}
			}
		}
		if (chosen_ability.mana != 0) { player.IncreaseMana(chosen_ability.mana); }
		if (chosen_ability.health != 0) { player.IncreaseHealth(chosen_ability.health); }
		if (chosen_ability.damage_boost != 0)
		{
			player.positive_effects.push_back(
				std::pair<std::string, std::pair<int, int>>("damage_boost", std::pair<int, int>(chosen_ability.damage_boost, chosen_ability.turns)));
		}
		if (chosen_ability.defense_boost != 0) 
		{
			player.positive_effects.push_back(
				std::pair<std::string, std::pair<int, int>>("defense_boost", std::pair<int, int>(chosen_ability.defense_boost, chosen_ability.turns)));
		}

	}
	else if (player_choice == "3")
	{
		//Проверка на присутствие итемов, которые можно использовать --> вывод текста/действие
	}
}

void EventHandler::NPCTurn(Player& player, NPC& npc)
{
	for (std::pair<std::string, std::pair<int, int>> effect : npc.positive_effects)
	{
		if (effect.second.second > 0) { effect.second.second -= 1; }
	}

	for (std::pair<std::string, std::pair<int, int>> effect : npc.negative_effects)
	{
		if (effect.second.second != 0)
		{
			if (effect.first == "periodic_damage" and effect.second.second > 0)
			{
				npc.IncreaseHealth(-effect.second.first);
				effect.second.second -= 1;
			}
		}
	}

	srand(time(NULL));
	int activity_choice = rand() % 2;
	if (activity_choice == 0)
	{
		float damage = npc.GetAttack();
		for (std::pair<std::string, std::pair<int, int>> effect : npc.positive_effects)
		{
			if (effect.first == "damage_boost" and effect.second.second > 0)
			{
				damage *= (1 + effect.second.first / 100);
			}
		}
		player.IncreaseHealth(-(int)damage);
		return;
	}

	bool spell_chosen = false;
	Ability chosen_ability;
	while (!spell_chosen)
	{
		std::cout << npc.GetAbilities().size() << std::endl;
		chosen_ability = npc.GetAbilities()[rand() % npc.GetAbilities().size()];

		if (chosen_ability.resource == "mana" and npc.GetMana() >= chosen_ability.resource_cost)
		{
			spell_chosen = true;
		}
		else if (chosen_ability.resource == "energy" and npc.GetEnergy() >= chosen_ability.resource_cost)
		{
			spell_chosen = true;
		}
		else if (npc.GetClassName() == "Warrior")
		{
			spell_chosen = true;
		}
	}

	if (chosen_ability.resource == "mana") { npc.IncreaseMana(-chosen_ability.resource_cost); }
	if (chosen_ability.resource == "energy") { npc.IncreaseEnergy(-chosen_ability.resource_cost); }
	if (chosen_ability.damage != 0)
	{
		float damage = chosen_ability.damage;
		for (std::pair<std::string, std::pair<int, int>> effect : npc.positive_effects)
		{
			if (effect.first == "damage_boost" and effect.second.second > 0)
			{
				damage *= (1 + effect.second.first / 100);
			}
		}

		player.IncreaseHealth(-(int)damage);

		if (chosen_ability.periodic_damage != 0)
		{
			player.negative_effects.push_back(
				std::pair<std::string, std::pair<int, int>>("periodic_damage", std::pair<int, int>(chosen_ability.periodic_damage, chosen_ability.turns)));
		}
	}
	if (chosen_ability.mana != 0) { npc.IncreaseMana(chosen_ability.mana); }
	if (chosen_ability.health != 0) { npc.IncreaseHealth(chosen_ability.health); }
	if (chosen_ability.damage_boost != 0)
	{
		npc.positive_effects.push_back(
			std::pair<std::string, std::pair<int, int>>("damage_boost", std::pair<int, int>(chosen_ability.damage_boost, chosen_ability.turns)));
	}
	if (chosen_ability.defense_boost != 0)
	{
		npc.positive_effects.push_back(
			std::pair<std::string, std::pair<int, int>>("defense_boost", std::pair<int, int>(chosen_ability.defense_boost, chosen_ability.turns)));
	}
}