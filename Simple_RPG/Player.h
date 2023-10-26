#pragma once
#include "Character.h"
#include "Race.h"
#include "GameClass.h"

class Player : public Character
{
private:

public:
	Player();
	Player(Race r, GameClass gc);
};