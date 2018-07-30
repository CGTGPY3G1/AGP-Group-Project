#include "AutoRotator.h"
#include "GameObject.h"
#include "Transform.h"
namespace B00289996B00227422 {
	AutoRotator::AutoRotator() {
	}

	AutoRotator::AutoRotator(std::weak_ptr<GameObject> gameObject) :ScriptableComponent(gameObject) {
	}

	AutoRotator::~AutoRotator() {
	}

	void AutoRotator::Update(const float & deltaTime) {
		// if the object has a transform component, rotate it by the alpha value around the axis
		std::shared_ptr<Transform> transform = GetComponent<Transform>().lock();
		if(transform) transform->Rotate(axis, alpha * deltaTime);
	}

	void AutoRotator::SetValues(const glm::vec3 & axis, const float & alpha) {
		this->axis = axis; this->alpha = alpha;
	}
	const glm::vec3 AutoRotator::GetAxis() const {
		return axis;
	}
	const float AutoRotator::GetAlpha() const {
		return alpha;
	}
}
