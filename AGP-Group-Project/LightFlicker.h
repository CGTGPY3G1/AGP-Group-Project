#pragma once
#ifndef B00289996B00227422_LIGHT_FLICKER_H
#define B00289996B00227422_LIGHT_FLICKER_H
#include "ScriptableComponent.h"
#include <glm\gtc\matrix_transform.hpp>

namespace B00289996B00227422 {
	class Light;
	class Transform;
	class LightFlicker : public ScriptableComponent {
	public:
		/// <summary> Initializes a new instance of the <see cref="LightFlicker"/> class. </summary>
		LightFlicker();
		/// <summary> Initializes a new instance of the <see cref="LightFlicker"/> class. </summary>
		/// <param name="gameObject">The game object this component is attached to.</param>
		LightFlicker(std::weak_ptr<GameObject> gameObject);
		/// <summary>Finalizes an instance of the <see cref="LightFlicker"/> class. </summary>
		~LightFlicker();
		const ComponentType Type() const override { return COMPONENT_LIGHT_FLICKER; }
		const std::string GetName() const override { return "LightFlicker"; }
		void Update(const float & deltaTime) override;
	private:
		/// <summary> Gets a new attenuation value. </summary>
		/// <param name="deltaTime">The frame time.</param>
		/// <returns> the new attenuation value</returns>
		float GetNewAttenuation(const float & deltaTime);
		glm::vec3 startPosition;
		std::weak_ptr<Transform> lightTransform;
		std::weak_ptr<Light> light;
		bool increasing = false;
		// minimum and maximum ranges for the attenuation value
		const float minMin = 0.65f, minMax = 0.75f, maxMin = 0.9f, maxMax = 1.0f;
		// current min and min targets, the current attenuation value and the speed of change
		float min, max, current, speed = 1.5f;
	};
}
#endif // !B00289996B00227422_LIGHT_FLICKER_H