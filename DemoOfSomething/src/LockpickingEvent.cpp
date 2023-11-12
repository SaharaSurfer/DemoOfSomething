#include "../header/LockpickingEvent.h"

int LockpickingEvent::Execute(json node, Character& player, Interface& interface)
{
	if (node["interactive_object_name"] == "Lock") {
		Lock lock = CreateLock(node);
		int result = AttemptToUnlock(lock, player, interface, node);
		return result;
	}

	interface.RenderText(std::string(node["none"]));
	return 0;
}

Lock LockpickingEvent::CreateLock(json node) 
{
	Lock lock;
	lock.SetLockLevel(node["security_level"]);
	return lock;
}

int LockpickingEvent::AttemptToUnlock(Lock& lock, Character& player, Interface& interface, json node) 
{
	if (player.inventory.CountItem("Lockpick") > 0) 
	{
		std::shared_ptr<Item> item = player.inventory.GetItem("Lockpick");
		std::shared_ptr<Lockpick> lockpick = std::dynamic_pointer_cast<Lockpick>(item);

		bool is_open = lockpick->UnlockObject(lock, player);
		if (is_open) 
		{
			interface.RenderText(std::string(node["success"]) + "\n");
			return 1;
		}

		HandleFailure(lockpick, player, interface, node);

		if (player.inventory.CountItem("Lockpick") == 0) 
		{
			return 0;
		}

		return 2;
	}

	interface.RenderText(std::string(node["none"]));
	return 0;
}

void LockpickingEvent::HandleFailure(std::shared_ptr<Lockpick> lockpick, Character& player, Interface& interface, json node) 
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, 2);

	interface.RenderText(std::string(node["fail"][distribution(gen)]) + "\n");
	player.inventory.DeleteItem("Lockpick");

	int lockpick_num = player.inventory.CountItem("Lockpick");
	std::string text =
		"<You have " +
		std::to_string(lockpick_num) +
		" of lock picks left>\n";
	interface.RenderText(text);
}
