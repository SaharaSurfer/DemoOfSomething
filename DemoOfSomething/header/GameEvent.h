#pragma once
#include "Character.h"
#include "Interface.h"
#include "json.hpp"

class GameEvent
{
public:
	virtual int Execute(json node, Character& player, Interface& interface) = 0;
	virtual ~GameEvent() = default;
};