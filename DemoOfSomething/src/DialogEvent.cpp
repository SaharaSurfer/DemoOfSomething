#include "../header/DialogEvent.h"

int DialogEvent::Execute(json node, Character& player, Interface& interface)
{
	json dialog = interface.LoadJSON(std::string(node["dialog_name"]));
	std::deque<json> current_options = InitializeOptions(dialog);

	while (!current_options.empty())
	{
		DisplayOptions(current_options);

		size_t choice = interface.CollectPlayerChoice(current_options.size());
		const json& chosen_node = current_options[choice];

		RenderLines(chosen_node, interface);

		HandleEndOption(current_options, chosen_node);
		EraseChosenOption(current_options, choice);

		AddOptions(current_options, chosen_node);
	}

	return 0;
}

std::deque<json> DialogEvent::InitializeOptions(const json& dialog)
{
	std::deque<json> current_options;
	for (const auto& option : dialog) {
		current_options.push_back(option);
	}
	return current_options;
}

void DialogEvent::DisplayOptions(const std::deque<json>& current_options)
{
	for (size_t i = 0; i < current_options.size(); i++) 
	{
		std::string text = std::to_string(i + 1) + ") " + std::string(current_options[i]["choice_name"]) + "\n";
		std::cout << text;
	}
}

void DialogEvent::RenderLines(const json& chosen_node, Interface& interface)
{
	interface.RenderText(std::string(chosen_node["hero_line"]) + "\n");
	interface.RenderText(std::string(chosen_node["npc_line"]) + "\n");
}

void DialogEvent::HandleEndOption(std::deque<json>& current_options, const json& chosen_node)
{
	if (chosen_node["is_end"] == 1) 
	{
		current_options.clear();
	}
}

void DialogEvent::EraseChosenOption(std::deque<json>& current_options, size_t choice)
{
	if (!current_options.empty()) 
	{
		current_options.erase(current_options.begin() + choice - 1);
	}
}

void DialogEvent::AddOptions(std::deque<json>& current_options, const json& chosen_node)
{
	if (chosen_node.contains("options")) 
	{
		for (const auto& node : chosen_node["options"]) 
		{
			current_options.push_back(node);
		}
	}
}
