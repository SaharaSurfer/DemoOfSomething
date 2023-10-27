#include "GamePlayHandler.h"

void from_json(const json& j, Race& r)
{
	j.at("strengthBonus").get_to(r.strengthBonus);
	j.at("dexterityBonus").get_to(r.dexterityBonus);
	j.at("intelligenceBonus").get_to(r.intelligenceBonus);
	j.at("wisdomBonus").get_to(r.wisdomBonus);
	j.at("charismaBonus").get_to(r.charismaBonus);
}

void from_json(const json& j, GameClass& gc)
{
	j.at("healthBonus").get_to(gc.healthBonus);
	j.at("manaBonus").get_to(gc.manaBonus);
	j.at("defenseBonus").get_to(gc.defenseBonus);
	j.at("attackBonus").get_to(gc.attackBonus);
}

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
			if (pair.value() != 0) 
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
			if (pair.value() != 0)
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

void GamePlayHandler::ProceedCharacterCreation()
{
	std::string entry = interface.LoadText("Text\\Entry.txt");
	interface.RenderText(entry);
	system("pause");
	system("cls");

	std::string character_creation = "Locations\\banner.txt";
	interface.LoadLocation(character_creation);
	interface.RenderLocation();

	// Бонусы класса добавляются за каждый уровень
	Race race_choice = ChoosingRace();
	GameClass class_choice = ChoosingClass();
	player = Player(race_choice, class_choice);
	system("cls");
}

void GamePlayHandler::TellJourneyStart()
{
	std::string journey_starts = interface.LoadText("Text\\Beginning_of_the_journey.txt");
	interface.RenderText(journey_starts);

	json escape_choice = interface.LoadJSON("JsonFiles\\Escape_choice.json");
	std::unordered_set<std::string> player_choices;

	while (player_choices.size() != escape_choice[0].size())
	{
		for (auto kv = escape_choice[0].begin(); kv != escape_choice[0].end(); kv++)
		{
			if (player_choices.count(kv.key()) == 0)
			{
				interface.RenderText(kv.key() + ") " + std::string(kv.value()) + "\n");
			}
		}

		std::string player_choice = "";
		std::getline(std::cin, player_choice);
		player_choice = player_choice.substr(0, 1);
		player_choices.insert(player_choice);

		interface.RenderText(escape_choice[1][player_choice]);
		system("pause");

		std::string is_non_return = escape_choice[2][player_choice];
		if (std::stoi(is_non_return) == 0) { break; }
	}
}