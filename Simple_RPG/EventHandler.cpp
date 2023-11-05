#include "EventHandler.h"

int EventHandler::HandleEvent(json node, Player& player)
{
	static const std::unordered_map<std::string, std::function<int(EventHandler&, json, Player&)>> eventHandlers = 
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

int EventHandler::ReceiveItems(json node, Player& player)
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
			player.AddItemToInventory(std::move(item));
		}
	}

	return 0;
}

int EventHandler::ProcessLockpicking(json node, Player& player)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, 2);

	json failure_text = node["fail"];

	if (node["interactive_object_name"] == "Lock")
	{
		Lock lock = Lock();
		lock.SetLockLevel(node["security_level"]);

		while (player.CountItemInInventory("Lockpick") > 0)
		{
			std::shared_ptr<Item> item = player.GetItemFromInventory("Lockpick");
			std::shared_ptr<Lockpick> lockpick = std::dynamic_pointer_cast<Lockpick>(item);

			bool is_open = lockpick -> UnlockObject(lock, player);
			if (is_open)
			{
				interface.RenderText(std::string(node["success"]) + "\n");
				return 1;
			}

			interface.RenderText(std::string(failure_text[distribution(gen)]) + "\n");
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
		Lock lock = Lock();
		lock.SetKeyName(std::string(node["key_needed"]));

		std::shared_ptr<Item> item = player.GetItemFromInventory(std::string(node["key_needed"]));
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

int EventHandler::ProcessDialog(json node, Player& player)
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
			interface.RenderText(text);
		}

		//Getting the player's solution
		size_t choice = interface.CollectPlayerChoice(current_options.size());
		if (choice < current_options.size())
		{
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
		else
		{
			interface.RenderText("Invalid choice. Please try again.\n");
		}
	}

	return 0;
}

int EventHandler::ProcessSurpriseAttack(json node, Player& player)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 20);

	if (distribution(gen) + player.GetStatBonus(node["stat_check"]) < node["complexity"])
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

	int player_initiative = GenerateInitiative();
	std::vector<int> enemy_initiatives = GenerateEnemyInitiative(enemies);
	
	std::vector<int> all_initiatives = enemy_initiatives;
	all_initiatives.push_back(player_initiative);
	std::sort(all_initiatives.rbegin(), all_initiatives.rend());

	while (AreAnyEnemyAlive(enemies))
	{
		for (int initiative: all_initiatives)
		{
			if (initiative == player_initiative)
			{
				OutputCurrentFightState(player, enemies);
				PlayerTurn(player, enemies);
			}
			else
			{
				int enemy_ind = std::find(enemy_initiatives.rbegin(), enemy_initiatives.rend(), 
					initiative) - enemy_initiatives.rbegin();
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
		Race r;

		std::string enemy_class = std::string(node["classes_of_enemy"][i]);
		std::string enemy_race = std::string(node["races_of_enemy"][i]);

		classes[0][enemy_class].get_to(gc);
		races[0][enemy_race].get_to(r);

		NPC enemy = NPC(r, gc, node["levels"][i]);
		enemies.push_back(enemy);
	}

	return enemies;
}

int EventHandler::GenerateInitiative()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 20);
	return distribution(gen);
}

std::vector<int> EventHandler::GenerateEnemyInitiative(const std::vector<NPC>& enemies)
{
	std::vector<int> enemy_initiatives;

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemy_initiatives.push_back(GenerateInitiative());
	}

	return enemy_initiatives;
}

bool EventHandler::AreAnyEnemyAlive(const std::vector<NPC>& enemies)
{
	return std::any_of(enemies.cbegin(), enemies.cend(), [](NPC npc) { return !npc.IsDead(); });
}

int EventHandler::GetEnemyIndexByInitiative(const std::vector<int>& enemy_initiatives, int initiative)
{
	auto it = std::find(enemy_initiatives.begin(), enemy_initiatives.end(), initiative);
	if (it != enemy_initiatives.end()) 
	{
		return std::distance(enemy_initiatives.begin(), it);
	}
}

void EventHandler::OutputCurrentFightState(Player& player, std::vector<NPC> enemies)
{
	const int health_pallets = 10;
	const int health_percent_padding = 4;
	const std::string separator_line = std::string(50, '-');

	std::cout << separator_line << "\n";
	std::cout << "Player Turn\n";
	std::cout << separator_line << "\n";
	
	auto PrintHealthInfo = [](Character& character) {
		int numPallets = (character.GetHealthPercent() + health_percent_padding) / health_pallets;
		std::string healthBar = std::string(numPallets, '#') +
			std::string(health_pallets - numPallets, '*');
		std::cout << character.GetName() << "(" << character.GetRaceName() << " " << character.GetClassName() << ")"
			<< ": " << healthBar << " " << character.GetHealth() << "/" << character.GetMaxHealth()
			<< " " << character.GetMana() << " mana " << character.GetEnergy() << " energy\n";
		};

	PrintHealthInfo(player);
	std::cout << separator_line << "\n";

	for (size_t i = 0; i < enemies.size(); i++)
	{
		PrintHealthInfo(enemies[i]);
	}

	std::cout << separator_line << "\n";
}

void EventHandler::PlayerTurn(Player& player, std::vector<NPC>& enemies)
{
	HandleEffects(player, player.positive_effects);
	HandleEffects(player, player.negative_effects);

	std::cout << "1) Attack with weapon in hand\n";
	std::cout << "2) Use ability\n";
	std::cout << "3) Use item\n";
	std::cout << "Enter your choice: ";

	size_t player_choice = interface.CollectPlayerChoice(3);
	if (player_choice == 0)
	{
		std::cout << "Enter target: ";
		size_t target_choice = interface.CollectPlayerChoice(enemies.size());

		float damage = CalculateBoostedDamage(player.GetAttack(), player.positive_effects);
		enemies[target_choice].IncreaseHealth(-static_cast<int>(damage));
	}
	else if (player_choice == 1)
	{
		int ability_ind = 0;
		for (Ability ability : player.GetAbilities())
		{
			ability_ind++;

			std::cout << ability_ind << ") ";
			ability.ShowInfo();
		}

		std::cout << "Enter ability choice: ";
		size_t ability_choice = interface.CollectPlayerChoice(player.GetAbilities().size());
		Ability chosen_ability = player.GetAbilities()[ability_choice];

		if (chosen_ability.resource == "mana") { player.IncreaseMana(-chosen_ability.resource_cost); }
		if (chosen_ability.resource == "energy") { player.IncreaseEnergy(-chosen_ability.resource_cost); }
		if (chosen_ability.damage != 0)
		{
			std::cout << "Enter target: ";
			size_t target_choice = interface.CollectPlayerChoice(enemies.size());

			float damage = CalculateBoostedDamage(chosen_ability.damage, player.positive_effects);
			enemies[target_choice].IncreaseHealth(-static_cast<int>(damage));

			if (chosen_ability.periodic_damage != 0)
			{
				enemies[target_choice].negative_effects.emplace_back("periodic_damage", std::make_pair(chosen_ability.periodic_damage, chosen_ability.turns));
			}
			
			if (chosen_ability.enemy != 0)
			{
				int enemy_ind = -1, damaged_enemies = 0;
				while (damaged_enemies < (chosen_ability.enemy - 1) and (enemy_ind + 1) < enemies.size())
				{
					enemy_ind++;
					if (enemy_ind == target_choice) { continue; }
					enemies[enemy_ind].IncreaseHealth(-static_cast<int>(damage));
					damaged_enemies++;
				}
			}
		}
		if (chosen_ability.mana != 0) { player.IncreaseMana(chosen_ability.mana); }
		if (chosen_ability.health != 0) { player.IncreaseHealth(chosen_ability.health); }
		if (chosen_ability.damage_boost != 0)
		{
			player.positive_effects.emplace_back("damage_boost", std::make_pair(chosen_ability.damage_boost, chosen_ability.turns));
		}
		if (chosen_ability.defense_boost != 0) 
		{
			player.positive_effects.emplace_back("defense_boost", std::make_pair(chosen_ability.damage_boost, chosen_ability.turns));
		}

	}
	else if (player_choice == 2)
	{
		//Проверка на присутствие итемов, которые можно использовать --> вывод текста/действие
	}
}

void EventHandler::NPCTurn(Player& player, NPC& npc)
{
	HandleEffects(npc, npc.positive_effects);
	HandleEffects(npc, npc.negative_effects);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> activityDistribution(0, 1);
	int activity_choice = activityDistribution(gen);
	if (activity_choice == 0)
	{
		float damage = CalculateBoostedDamage(npc.GetAttack(), npc.positive_effects);
		player.IncreaseHealth(-static_cast<int>(damage));
		return;
	}

	std::uniform_int_distribution<int> abilityDistribution(0, static_cast<int>(npc.GetAbilities().size()) - 1);
	Ability chosen_ability;
	bool spell_chosen = false;
	while (!spell_chosen)
	{
		int ability_ind = abilityDistribution(gen);
		chosen_ability = npc.GetAbilities()[ability_ind];

		if (chosen_ability.resource == "mana" and npc.GetMana() >= chosen_ability.resource_cost or
			chosen_ability.resource == "energy" and npc.GetEnergy() >= chosen_ability.resource_cost or
			npc.GetClassName() == "Warrior")
		{
			spell_chosen = true;
		}
	}

	if (chosen_ability.resource == "mana") { npc.IncreaseMana(-chosen_ability.resource_cost); }
	if (chosen_ability.resource == "energy") { npc.IncreaseEnergy(-chosen_ability.resource_cost); }
	if (chosen_ability.damage != 0)
	{
		float damage = CalculateBoostedDamage(chosen_ability.damage, npc.positive_effects);
		player.IncreaseHealth(-static_cast<int>(damage));

		if (chosen_ability.periodic_damage != 0)
		{
			player.negative_effects.emplace_back("periodic_damage", std::make_pair(chosen_ability.periodic_damage, chosen_ability.turns));
		}
	}
	if (chosen_ability.mana != 0) { npc.IncreaseMana(chosen_ability.mana); }
	if (chosen_ability.health != 0) { npc.IncreaseHealth(chosen_ability.health); }
	if (chosen_ability.damage_boost != 0)
	{
		npc.positive_effects.emplace_back("damage_boost", std::make_pair(chosen_ability.damage_boost, chosen_ability.turns));
	}
	if (chosen_ability.defense_boost != 0)
	{
		npc.positive_effects.emplace_back("defense_boost", std::make_pair(chosen_ability.defense_boost, chosen_ability.turns));
	}
}

void EventHandler::HandleEffects(Character& character, std::vector<std::pair<std::string, std::pair<int, int>>>& effects)
{
	for (auto& effect : effects)
	{
		if (effect.second.second > 0)
		{
			effect.second.second -= 1;

			if (effect.first == "periodic_damage")
			{
				character.IncreaseHealth(-effect.second.first);
			}
		}
	}
}

float EventHandler::CalculateBoostedDamage(float base_damage, std::vector<std::pair<std::string, std::pair<int, int>>>& effects)
{
	float damage = base_damage;
	for (const auto& effect : effects) 
	{
		if (effect.first == "damage_boost" && effect.second.second > 0)
		{
			damage *= (1 + effect.second.first / 100.0);
		}
	}
	return damage;
}