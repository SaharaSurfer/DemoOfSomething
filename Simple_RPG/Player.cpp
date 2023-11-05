#include "Player.h"

Player::Player() {}

Player::Player(const Race& r, const GameClass& gc) : Character(r, gc) {}

std::vector<Ability> Player::GetAbilities()
{
	return abilities;
}