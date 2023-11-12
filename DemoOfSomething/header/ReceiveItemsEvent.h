#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "GameEvent.h"
#include "Key.h"

class ReceiveItemsEvent : public GameEvent
{
private:
	static const std::unordered_map<std::string, std::function<std::shared_ptr<Item>()>> item_creators;

public:
	int Execute(json node, Character& player, Interface& interface) override;
};