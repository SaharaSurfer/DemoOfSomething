#include "Player.h"

Player::Player() {}

Player::Player(Race r, GameClass gc)
{
	race = r;
	gameclass = gc;

	strength += r.strengthBonus;
	dexterity += r.dexterityBonus;
	intelligence += r.intelligenceBonus;
	wisdom += r.wisdomBonus;
	charisma += r.charismaBonus;

	health += gc.healthBonus * level;
	mana += gc.manaBonus * level;
	defense += gc.defenseBonus * level;
	attack += gc.attackBonus * level;
}