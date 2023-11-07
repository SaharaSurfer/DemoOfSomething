#include "GamePlayHandler.h"

static void ClearConsole(void)
{
	system("cls||clear");
	return;
}

GamePlayHandler::GamePlayHandler() {}

Race GamePlayHandler::ChoosingRace(const json& races)
{
	interface.RenderText("Choose your race: \n");

	int race_index = 0;
	for (auto kv : races[0].items())
	{
		race_index++;
		interface.RenderText(std::to_string(race_index) + ") " + kv.key() + ": ");
		DisplayRaceBonuses(kv.value());
	}

	std::string race_choice = std::to_string(interface.CollectPlayerChoice(races[0].size()) + 1);
	return LoadRace(races, race_choice);
}

void GamePlayHandler::DisplayRaceBonuses(const json& race_details)
{
	for (auto& pair : race_details.items()) 
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

Race GamePlayHandler::LoadRace(const json& races, const std::string& race_choice)
{
	Race race;
	std::string player_race_choice = races[1][race_choice];
	race.name = player_race_choice;
	races[0][player_race_choice].get_to(race);
	
	return race;
}

GameClass GamePlayHandler::ChoosingClass(const json& classes)
{
	interface.RenderText("Choose your class: \n");

	int class_index = 0;
	for (auto kv : classes[0].items())
	{
		class_index++;
		interface.RenderText(std::to_string(class_index) + ") " + kv.key() + ": ");
		DisplayClassBonuses(kv.value());
	}

	std::string class_choice = std::to_string(interface.CollectPlayerChoice(classes[0].size()) + 1);
	return LoadClass(classes, class_choice);
}

void GamePlayHandler::DisplayClassBonuses(const json& class_details)
{
	for (auto& pair : class_details.items()) 
	{
		if (pair.value() != 0 && pair.key() != "name") 
		{
			std::string value = std::to_string((int)pair.value());
			std::string key = pair.key().substr(0, pair.key().find('B'));
			interface.RenderText("+" + value + " " + key + "; ");
		}
	}
	std::cout << "\n";
}

GameClass GamePlayHandler::LoadClass(const json& classes, const std::string& class_choice)
{
	GameClass game_class;
	std::string player_class_choice = classes[1][class_choice];
	game_class.name = player_class_choice;
	classes[0][player_class_choice].get_to(game_class);

	return game_class;
}

void GamePlayHandler::CreatePlayerCharacter()
{
	DisplayIntroductoryText("Text\\Entry.txt");
	DisplayCharacterCreationScreen("Locations\\CharacterCreation.txt");

	Race race_choice = ChoosingRace(interface.LoadJSON("JsonFiles\\Race.json"));
	GameClass class_choice = ChoosingClass(interface.LoadJSON("JsonFiles\\Class.json"));
	player = Character(race_choice, class_choice);
	ClearConsole();
}

void GamePlayHandler::DisplayIntroductoryText(const std::string& filename)
{
	std::string entry = interface.LoadText(filename);
	interface.RenderText(entry);
	ClearConsole();
}

void GamePlayHandler::DisplayCharacterCreationScreen(const std::string& filename)
{
	interface.LoadLocation(filename);
	interface.RenderLocation();
}

void GamePlayHandler::ProcessDecisionTree(const json& tree)
{
	std::vector<json> nodes_in_use;

	//Filling in initial options
	for (auto& node : tree)
	{
		nodes_in_use.push_back(node);
	}

	//Starting to process the player's decisions
	while (!nodes_in_use.empty())
	{
		DisplayPlayerChoices(nodes_in_use);

		//Getting the player's solution
		size_t player_choice = interface.CollectPlayerChoice(nodes_in_use.size());
		json chosen_node = nodes_in_use[player_choice];
		player.GainExp(chosen_node["experience"]);

		/*If the player's choices led to a plot turning point/end game,
		then discard the rest of the branches*/
		if (chosen_node["returnable"] == 0)
		{
			nodes_in_use.clear();
		}

		//If the player's choice caused an event to occur, we process it
		int event_flag = ProcessEventAndText(chosen_node);
		if (event_flag == 1) { break; }
		else if (event_flag == 2) { continue; }

		//Removing already used branch
		if (!nodes_in_use.empty())
		{
			nodes_in_use.erase(nodes_in_use.begin() + player_choice);
		}
		
		//Recording new branches if any
		AddNewBranches(nodes_in_use, chosen_node);
	}
}

void GamePlayHandler::DisplayPlayerChoices(const std::vector<json>& nodes_in_use)
{
	for (size_t i = 0; i < nodes_in_use.size(); i++)
	{
		std::string text = std::to_string(i + 1) + ") " + std::string(nodes_in_use[i]["choice_name"]) + "\n";
		interface.RenderText(text);
	}
}

int GamePlayHandler::ProcessEventAndText(const json& chosen_node)
{
	int event_flag = -1;
	if (chosen_node["event"] != "0")
	{
		event_flag = event_handler.HandleEvent(chosen_node, player);
	}

	if (chosen_node.contains("text"))
	{
		interface.RenderText(std::string(chosen_node["text"]) + "\n\n");
	}

	return event_flag;
}

void GamePlayHandler::AddNewBranches(std::vector<json>& nodes_in_use, const json& chosen_node)
{
	if (chosen_node.contains("options"))
	{
		for (auto& node : chosen_node["options"])
		{
			nodes_in_use.push_back(node);
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