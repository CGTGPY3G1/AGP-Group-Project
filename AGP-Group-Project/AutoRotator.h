#pragma once
#ifndef B00289996_AUTO_ROTATE_H
#define B00289996_AUTO_ROTATE_H
#include "ScriptableComponent.h"
#include <glm\gtc\matrix_transform.hpp>

namespace B00289996 {
	class AutoRotator : public ScriptableComponent {
	public:
		AutoRotator();
		AutoRotator(std::weak_ptr<GameObject> gameObject);
		~AutoRotator();
		const ComponentType Type() const override { return COMPONENT_AUTO_ROTATOR; }
		const std::string GetName() const override { return "AutoRotator"; }
		void Update(const float & deltaTime) override;
		void SetValues(const glm::vec3 & axis, const float & alpha);
		const glm::vec3 GetAxis() const;
		const float GetAlpha() const;
	private:
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
		float alpha = 90.0f;
	};
}
#endif // !B00289996_AUTO_ROTATE_H