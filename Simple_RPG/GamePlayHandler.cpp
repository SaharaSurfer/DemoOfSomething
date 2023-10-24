#include "GamePlayHandler.h"

Race GamePlayHandler::ChoosingRace()
{
	interface.RenderText("Choose your race: \n");
	json races = interface.LoadJSON("JsonFiles\\Race.json");

	int race_index = 0;
	for (auto kv = races.begin(); kv != races.end(); kv++)
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
	//Найти способ получить доступ к расе по индексу.
	//Записать через go_to в race
	//auto k = races.at("Drow");
	//std::cout << k;

	return race;
}
GameClass GamePlayHandler::ChoosingClass()
{
	interface.RenderText("Choose your class: \n");
	json classes = interface.LoadJSON("JsonFiles\\Class.json");

	int class_index = 0;
	for (auto kv = classes.begin(); kv != classes.end(); kv++)
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

	return game_class;
}

void GamePlayHandler::ProceedCharacterCreation()
{
	/*std::string entry = interface.LoadText("Text\\Entry.txt");
	interface.RenderText(entry);
	system("pause");

	system("cls");
	std::string character_creation = "Locations\\banner.txt";
	interface.LoadLocation(character_creation);
	interface.RenderLocation();*/

	// Бонусы класса добавляются за каждый уровень
	Race race_choice = ChoosingRace();
	GameClass class_choice = ChoosingClass();
}