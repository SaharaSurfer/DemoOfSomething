#pragma once
#include "Interface.h"
#include "Character.h"
#include "Lockpick.h"
#include "Key.h"
#include "Lock.h"
#include "EventHandler.h"

class GamePlayHandler
{
private:
	Interface& interface;
	EventHandler event_handler{interface};
	Character player;

	Race ChoosingRace(const json& races);
	void DisplayRaceBonuses(const json& race_details);
	Race LoadRace(const json& races, const std::string& race_choice);

	GameClass ChoosingClass(const json& classes);
	void DisplayClassBonuses(const json& class_details);
	GameClass LoadClass(const json& classes, const std::string& class_choice);

	void DisplayIntroductoryText(const std::string& filename);
	void DisplayCharacterCreationScreen(const std::string& filename);

	void ProcessDecisionTree(const json& tree);
	void DisplayPlayerChoices(const std::vector<json>& nodes_in_use);
	int ProcessEventAndText(const json& chosen_node);
	void AddNewBranches(std::vector<json>& nodes_in_use, const json& chosen_node);

public:
	GamePlayHandler(Interface& intf);

	void CreatePlayerCharacter();
	void EscapeFromCage();
};