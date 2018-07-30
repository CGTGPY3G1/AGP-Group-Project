#pragma once
#ifndef B00289996B00227422_GAMEOBJECT_DATA_H
#define B00289996B00227422_GAMEOBJECT_DATA_H
#include "GameObject.h"
namespace B00289996B00227422 {
	/// <summary>
	/// Used to prevent the cast from weak_ptr to void * invalidating the weak_ptr
	/// </summary>
	struct GameObjectData {
		std::weak_ptr<GameObject> gameObject;
	};
}
#endif // !B00289996B00227422_GAMEOBJECT_DATA_H


