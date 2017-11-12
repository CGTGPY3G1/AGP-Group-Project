#pragma once
#ifndef B00289996_LIGHT_FLICKER_H
#define B00289996_LIGHT_FLICKER_H
#include "ScriptableComponent.h"
#include <glm\gtc\matrix_transform.hpp>

namespace B00289996 {
	class Light;
	class Transform;
	class LightFlicker : public ScriptableComponent {
	public:
		LightFlicker();
		LightFlicker(std::weak_ptr<GameObject> gameObject);
		~LightFlicker();
		const ComponentType Type() const override { return COMPONENT_LIGHT_FLICKER; }
		const std::string GetName() const override { return "LightFlicker"; }
		void Update(const float & deltaTime) override;
	private:
		float GetNewAttenuation(const float & deltaTime);
		glm::vec3 startPosition;
		std::weak_ptr<Transform> lightTransform;
		std::weak_ptr<Light> light;
		bool increasing = false;
		const float minMin = 0.65f, minMax = 0.75f, maxMin = 0.9f, maxMax = 1.0f;
		float min, max, current, speed = 1.5f;
	};
}
#endif // !B00289996_LIGHT_FLICKER_H