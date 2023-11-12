#include "../header/UseKeyEvent.h"

int UseKeyEvent::Execute(json node, Character& player, Interface& interface)
{
	if (node["interactive_object_name"] == "Lock") {
		Lock lock = CreateLock(node);

		if (UnlockWithKey(lock, player, interface, node)) {
			interface.RenderText(std::string(node["success"]));
		}
		else {
			interface.RenderText(std::string(node["none"]));
			return 1;
		}
	}

	return 0;
}

Lock UseKeyEvent::CreateLock(json node)
{
	Lock lock;
	lock.SetKeyName(std::string(node["key_needed"]));
	return lock;
}

bool UseKeyEvent::UnlockWithKey(Lock& lock, Character& player, Interface&, json node)
{
	std::shared_ptr<Item> item = player.inventory.GetItem(std::string(node["key_needed"]));
	if (auto key = std::dynamic_pointer_cast<Key>(item)) {
		key->UnlockObject(lock);
		return true;
	}

	return false;
}
