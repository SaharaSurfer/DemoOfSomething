#pragma once
#include <random>
#include "GameEvent.h"

class SurpriseAttackEvent : public GameEvent
{
private:

public:
	int Execute(json node, Character& player, Interface& interface) override;
};