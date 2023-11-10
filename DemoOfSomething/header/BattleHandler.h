#pragma once
#include <unordered_map>
#include <functional>
#include <deque>
#include <random>
#include "Interface.h"
#include "Character.h"

class BattleHandler
{
private:
	Interface& interface;
	Character& player;
	std::vector<Character> enemies;

	std::vector<Character> CreateNPCs(json node);
	int GenerateInitiative();
	std::vector<int> GenerateEnemyInitiative(const std::vector<Character>& enemies);
	bool AreAnyEnemyAlive(const std::vector<Character>& enemies);
	int GetEnemyIndexByInitiative(const std::vector<int>& enemy_initiatives, int initiative);
	void OutputCurrentFightState(Character& player, std::vector<Character> enemies);
	void PlayerTurn(Character& player, std::vector<Character>& enemies);
	void NPCTurn(Character& player, Character& npc);

	void HandleEffects(Character& character, std::vector<std::pair<std::string, std::pair<int, int>>>& effects);
	float CalculateBoostedDamage(float base_damage, std::vector<std::pair<std::string, std::pair<int, int>>>& effects);

public:
	BattleHandler(json node, Character& hero, Interface& intf);

	int HandleFight();
};