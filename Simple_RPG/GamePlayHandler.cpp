#include "GamePlayHandler.h"

GamePlayHandler::GamePlayHandler() {}

Race GamePlayHandler::ChoosingRace()
{
	interface.RenderText("Choose your race: \n");
	json races = interface.LoadJSON("JsonFiles\\Race.json");

	int race_index = 0;
	for (auto kv = races[0].begin(); kv != races[0].end(); kv++)
	{
		race_index++;
		interface.RenderText(std::to_string(race_index) + ") " + kv.key() + ": ");
		for (auto& pair : kv.value().items())
		{
			if (pair.value() != 0 and pair.key() != "name")
			{
				std::string value = std::to_string((int)pair.value());
				std::string key = pair.key().substr(0, pair.key().find('B'));
				interface.RenderText("+" + value + " " + key + "; ");
			}
		}
		std::cout << "\n";
	}

	std::string player_race_choice;
	std::getline(std::cin, player_race_choice);

	Race race;
	player_race_choice = races[1][player_race_choice.substr(0, 1)];
	race.name = player_race_choice;
	races[0][player_race_choice].get_to(race);

	return race;
}
GameClass GamePlayHandler::ChoosingClass()
{
	interface.RenderText("Choose your class: \n");
	json classes = interface.LoadJSON("JsonFiles\\Class.json");

	int class_index = 0;
	for (auto kv = classes[0].begin(); kv != classes[0].end(); kv++)
	{
		class_index++;
		interface.RenderText(std::to_string(class_index) + ") " + kv.key() + ": ");
		for (auto& pair : kv.value().items())
		{
			if (pair.value() != 0 and pair.key() != "name")
			{
				std::string value = std::to_string((int)pair.value());
				std::string key = pair.key().substr(0, pair.key().find('B'));
				interface.RenderText("+" + value + " " + key + "; ");
			}
		}
		std::cout << "\n";
	}

	std::string player_class_choice;
	std::getline(std::cin, player_class_choice);

	GameClass game_class;
	player_class_choice = classes[1][player_class_choice.substr(0, 1)];
	game_class.name = player_class_choice;
	classes[0][player_class_choice].get_to(game_class);

	return game_class;
}

void GamePlayHandler::CreatePlayerCharacter()
{
	//Display the introductory text
	std::string entry = interface.LoadText("Text\\Entry.txt");
	interface.RenderText(entry);
	system("pause");
	system("cls");

	//Display the ASCII art for the character creation screen
	std::string character_creation = "Locations\\banner.txt";
	interface.LoadLocation(character_creation);
	interface.RenderLocation();

	// Creating a player character
	Race race_choice = ChoosingRace();
	GameClass class_choice = ChoosingClass();
	player = Player(race_choice, class_choice);
	system("cls");
}

void GamePlayHandler::ProcessDecisionTree(json tree)
{
	std::vector<json> nodes_in_use;

	//Filling in initial options
	for (auto& node : tree)
	{
		nodes_in_use.push_back(node);
	}

	//Starting to process the player's decisions
	while (nodes_in_use.size() > 0)
	{
		//Bringing up the possible player choices
		for (int i = 0; i < nodes_in_use.size(); i++)
		{
			std::string text = std::to_string(i + 1) + ") " + std::string(nodes_in_use[i]["choice_name"]) + "\n";
			interface.RenderText(text);
		}

		//Getting the player's solution
		std::string player_choice;
		std::getline(std::cin, player_choice);
		player_choice = player_choice.substr(0, 1);

		json chosen_node = nodes_in_use[std::stoi(player_choice) - 1];
		player.GainExp(chosen_node["experience"]);

		/*If the player's choices led to a plot turning point/end game,
		then discard the rest of the branches*/
		if (chosen_node["returnable"] == 0)
		{
			nodes_in_use.clear();
		}

		//If the player's choice caused an event to occur, we process it
		if (chosen_node["event"] != "0")
		{
			int is_over = event_handler.HandleEvent(chosen_node, player);
			if (is_over == 1) { break; }
			else if (is_over == 2) { continue; }
			
			// 0 - ...
			// 1 - break
			// 2 - retry
		}

		if (chosen_node.contains("text"))
		{
			interface.RenderText(std::string(chosen_node["text"]) + "\n\n");
		}

		//Removing already used branch
		if (nodes_in_use.size() != 0)
		{
			nodes_in_use.erase(nodes_in_use.begin() + std::stoi(player_choice) - 1);
		}
		
		//Recording new branches if any
		if (chosen_node.contains("options"))
		{
			for (auto& node : chosen_node["options"])
			{
				nodes_in_use.push_back(node);
			}
		}
	}
}

void GamePlayHandler::EscapeFromCage()
{
	std::string journey_starts = interface.LoadText("Text\\Beginning_of_the_journey.txt");
	interface.RenderText(journey_starts);

	json escape_choice = interface.LoadJSON("JsonFiles\\Escape.json");
	ProcessDecisionTree(escape_choice);
}