#include "../header/SurpriseAttackEvent.h"

int SurpriseAttackEvent::Execute(json node, Character& player, Interface& interface)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 20);

	if (distribution(gen) + player.characteristics.GetStatBonus(node["stat_check"]) < node["complexity"])
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