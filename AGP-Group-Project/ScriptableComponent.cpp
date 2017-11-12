#include "ScriptableComponent.h"

namespace B00289996 {
	ScriptableComponent::ScriptableComponent() {
	}

	ScriptableComponent::ScriptableComponent(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {

	}
}
