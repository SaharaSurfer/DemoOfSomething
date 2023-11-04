#pragma once
#include "Character.h"
#include "Race.h"
#include "GameClass.h"
#include "Interface.h"

class Player : public Character
{
private:

public:
	Player();
	Player(Race r, GameClass gc);

	std::vector<Ability> GetAbilities();
};