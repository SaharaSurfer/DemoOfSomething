#pragma once
#include <unordered_map>
#include <functional>
#include <deque>
#include "Interface.h"
#include "Player.h"
#include "NPC.h"
#include "Lockpick.h"
#include "Key.h"
#include "Lock.h"

class EventHandler
{
private:
	Interface interface;

	int ReceiveItems(json node, Player& player);
	int ProcessLockpicking(json node, Player& player);
	int UseKeyToOpen(json node, Player& player);
	int ProcessDialog(json node, Player& player);
	int ProcessSurpriseAttack(json node, Player& player);
	int ProcessFight(json node, Player& player);

	std::vector<NPC> CreateNPCs(json node);
	int GenerateInitiative();
	std::vector<int> GenerateEnemyInitiative(const std::vector<NPC>& enemies);
	bool AreAnyEnemyAlive(const std::vector<NPC>& enemies);
	int GetEnemyIndexByInitiative(const std::vector<int>& enemy_initiatives, int initiative);
	void OutputCurrentFightState(Player& player, std::vector<NPC> enemies);
	void PlayerTurn(Player& player, std::vector<NPC>& enemies);
	void NPCTurn(Player& player, NPC& npc);

	void HandleEffects(Character& character, std::vector<std::pair<std::string, std::pair<int, int>>>& effects);
	float CalculateBoostedDamage(float base_damage, std::vector<std::pair<std::string, std::pair<int, int>>>& effects);

public:
	int HandleEvent(json node, Player& player);
};