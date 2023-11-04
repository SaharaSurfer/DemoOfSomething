#pragma once
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
	int ProcessDialog(json node);
	int ProcessSurpriseAttack(json node, Player& player);
	int ProcessFight(json node, Player& player);

	std::vector<NPC> CreateNPCs(json node);
	void OutputCurrentFightState(Player& player, std::vector<NPC> enemies);
	void PlayerTurn(Player& player, std::vector<NPC>& enemies);
	void NPCTurn(Player& player, NPC& npc);

public:
	int HandleEvent(json node, Player& player);
};