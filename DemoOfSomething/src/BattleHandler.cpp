#include "../header/GamePlayHandler.h"

BattleHandler::BattleHandler(json node, Character& hero) : player(hero)
{
	enemies = CreateNPCs(node);
}

int BattleHandler::HandleFight()
{
	int player_initiative = GenerateInitiative();
	std::vector<int> enemy_initiatives = GenerateEnemyInitiative(enemies);

	std::vector<int> all_initiatives = enemy_initiatives;
	all_initiatives.push_back(player_initiative);
	std::sort(all_initiatives.rbegin(), all_initiatives.rend());

	while (AreAnyEnemyAlive(enemies))
	{
		if (player.IsDead())
		{
			return 1;
		}

		for (int initiative : all_initiatives)
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

	return 0;
}

std::vector<Character> BattleHandler::CreateNPCs(json node)
{
	json classes = interface.LoadJSON("JsonFiles\\Class.json");
	json races = interface.LoadJSON("JsonFiles\\Race.json");

	std::vector<Character> enemies;
	for (int i = 0; i < node["num_of_enemy"]; i++)
	{
		GameClass gc;
		Race r;

		std::string enemy_class = std::string(node["classes_of_enemy"][i]);
		std::string enemy_race = std::string(node["races_of_enemy"][i]);

		classes[0][enemy_class].get_to(gc);
		races[0][enemy_race].get_to(r);

		Character enemy = Character(r, gc, node["levels"][i]);
		enemies.push_back(enemy);
	}

	return enemies;
}

int BattleHandler::GenerateInitiative()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 20);
	return distribution(gen);
}

std::vector<int> BattleHandler::GenerateEnemyInitiative(const std::vector<Character>& enemies)
{
	std::vector<int> enemy_initiatives;

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemy_initiatives.push_back(GenerateInitiative());
	}

	return enemy_initiatives;
}

bool BattleHandler::AreAnyEnemyAlive(const std::vector<Character>& enemies)
{
	return std::any_of(enemies.cbegin(), enemies.cend(), [](Character npc) { return !npc.IsDead(); });
}

int BattleHandler::GetEnemyIndexByInitiative(const std::vector<int>& enemy_initiatives, int initiative)
{
	auto it = std::find(enemy_initiatives.begin(), enemy_initiatives.end(), initiative);
	if (it != enemy_initiatives.end())
	{
		return std::distance(enemy_initiatives.begin(), it);
	}
}

void BattleHandler::OutputCurrentFightState(Character& player, std::vector<Character> enemies)
{
	const std::string separator_line = std::string(50, '-');

	std::cout << separator_line << "\n";
	std::cout << "Player Turn\n";
	std::cout << separator_line << "\n";

	std::cout << player.GetResourceInfo();
	std::cout << separator_line << "\n";

	for (size_t i = 0; i < enemies.size(); i++)
	{
		std::cout << enemies[i].GetResourceInfo();
	}

	std::cout << separator_line << "\n";
}

void BattleHandler::PlayerTurn(Character& player, std::vector<Character>& enemies)
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

		float damage = CalculateBoostedDamage(player.characteristics.GetAttack(), player.positive_effects);
		enemies[target_choice].characteristics.IncreaseHealth(-static_cast<int>(damage), enemies[target_choice].positive_effects);
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

		if (chosen_ability.resource == "mana") { player.characteristics.IncreaseMana(-chosen_ability.resource_cost); }
		if (chosen_ability.resource == "energy") { player.characteristics.IncreaseEnergy(-chosen_ability.resource_cost); }
		if (chosen_ability.damage != 0)
		{
			std::cout << "Enter target: ";
			size_t target_choice = interface.CollectPlayerChoice(enemies.size());

			float damage = CalculateBoostedDamage(chosen_ability.damage, player.positive_effects);
			enemies[target_choice].characteristics.IncreaseHealth(-static_cast<int>(damage), enemies[target_choice].positive_effects);

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
					enemies[enemy_ind].characteristics.IncreaseHealth(-static_cast<int>(damage), enemies[enemy_ind].positive_effects);
					damaged_enemies++;
				}
			}
		}
		if (chosen_ability.mana != 0) { player.characteristics.IncreaseMana(chosen_ability.mana); }
		if (chosen_ability.health != 0) { player.characteristics.IncreaseHealth(chosen_ability.health, player.positive_effects); }
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

void BattleHandler::NPCTurn(Character& player, Character& npc)
{
	HandleEffects(npc, npc.positive_effects);
	HandleEffects(npc, npc.negative_effects);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> activityDistribution(0, 1);
	int activity_choice = activityDistribution(gen);
	if (activity_choice == 0)
	{
		float damage = CalculateBoostedDamage(npc.characteristics.GetAttack(), npc.positive_effects);
		player.characteristics.IncreaseHealth(-static_cast<int>(damage), player.positive_effects);
		return;
	}

	std::uniform_int_distribution<int> abilityDistribution(0, static_cast<int>(npc.GetAbilities().size()) - 1);
	Ability chosen_ability;
	bool spell_chosen = false;
	while (!spell_chosen)
	{
		int ability_ind = abilityDistribution(gen);
		chosen_ability = npc.GetAbilities()[ability_ind];

		if (chosen_ability.resource == "mana" and npc.characteristics.GetMana() >= chosen_ability.resource_cost or
			chosen_ability.resource == "energy" and npc.characteristics.GetEnergy() >= chosen_ability.resource_cost or
			npc.characteristics.GetClassName() == "Warrior")
		{
			spell_chosen = true;
		}
	}

	if (chosen_ability.resource == "mana") { npc.characteristics.IncreaseMana(-chosen_ability.resource_cost); }
	if (chosen_ability.resource == "energy") { npc.characteristics.IncreaseEnergy(-chosen_ability.resource_cost); }
	if (chosen_ability.damage != 0)
	{
		float damage = CalculateBoostedDamage(chosen_ability.damage, npc.positive_effects);
		player.characteristics.IncreaseHealth(-static_cast<int>(damage), player.positive_effects);

		if (chosen_ability.periodic_damage != 0)
		{
			player.negative_effects.emplace_back("periodic_damage", std::make_pair(chosen_ability.periodic_damage, chosen_ability.turns));
		}
	}
	if (chosen_ability.mana != 0) { npc.characteristics.IncreaseMana(chosen_ability.mana); }
	if (chosen_ability.health != 0) { npc.characteristics.IncreaseHealth(chosen_ability.health, npc.positive_effects); }
	if (chosen_ability.damage_boost != 0)
	{
		npc.positive_effects.emplace_back("damage_boost", std::make_pair(chosen_ability.damage_boost, chosen_ability.turns));
	}
	if (chosen_ability.defense_boost != 0)
	{
		npc.positive_effects.emplace_back("defense_boost", std::make_pair(chosen_ability.defense_boost, chosen_ability.turns));
	}
}

void BattleHandler::HandleEffects(Character& character, std::vector<std::pair<std::string, std::pair<int, int>>>& effects)
{
	for (auto& effect : effects)
	{
		if (effect.second.second > 0)
		{
			effect.second.second -= 1;

			if (effect.first == "periodic_damage")
			{
				character.characteristics.IncreaseHealth(-effect.second.first, character.positive_effects);
			}
		}
	}
}

float BattleHandler::CalculateBoostedDamage(float base_damage, std::vector<std::pair<std::string, std::pair<int, int>>>& effects)
{
	double damage = base_damage;
	for (const auto& effect : effects)
	{
		if (effect.first == "damage_boost" && effect.second.second > 0)
		{
			damage *= (1 + effect.second.first / 100.0);
		}
	}
	return damage;
}
