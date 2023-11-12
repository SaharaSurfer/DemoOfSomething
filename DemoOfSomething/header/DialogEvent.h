#pragma once
#include <deque>
#include "GameEvent.h"

class DialogEvent : public GameEvent
{
private:
	std::deque<json> InitializeOptions(const json& dialog);
	void DisplayOptions(const std::deque<json>& current_options);
	void RenderLines(const json& chosen_node, Interface& interface);
	void HandleEndOption(std::deque<json>& current_options, const json& chosen_node);
	void EraseChosenOption(std::deque<json>& current_options, size_t choice);
	void AddOptions(std::deque<json>& current_options, const json& chosen_node);

public:
	int Execute(json node, Character& player, Interface& interface) override;
};