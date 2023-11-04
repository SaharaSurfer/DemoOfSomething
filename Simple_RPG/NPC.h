#pragma once
#include "Character.h"
#include "Race.h"
#include "GameClass.h"
#include "Interface.h"

class NPC : public Character
{
private:

public:
	NPC();
	NPC(Race r, GameClass gc, int lvl);

	bool IsDead();

	std::string GetRaceName();
	std::string GetClassName();

	std::vector<Ability> GetAbilities();
};