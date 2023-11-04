#include "Key.h"

Key::Key()
{
	LoadData("Key");
}

bool Key::UnlockObject(Lock& object)
{
	if (object.GetKeyName() == name)
	{
		object.SetLockLevel(0);
		return 1;
	}
	else
	{
		interface.RenderText("<This key doesn't fit!>");
		return 0;
	}
}