#pragma once
#ifndef B00289996B00227422_AUTO_ROTATE_H
#define B00289996B00227422_AUTO_ROTATE_H
#include "ScriptableComponent.h"
#include <glm\gtc\matrix_transform.hpp>

namespace B00289996B00227422 {
	class AutoRotator : public ScriptableComponent {
	public:
		/// <summary> Initializes a new instance of the <see cref="AutoRotator"/> class. </summary>
		AutoRotator();
		/// <summary> Initializes a new instance of the <see cref="AutoRotator"/> class. </summary>
		/// <param name="gameObject">The game object this component is attached to.</param>
		AutoRotator(std::weak_ptr<GameObject> gameObject);
		/// <summary> Finalizes an instance of the <see cref="AutoRotator"/> class. </summary>
		~AutoRotator();
		const ComponentType Type() const override { return COMPONENT_AUTO_ROTATOR; }
		const std::string GetName() const override { return "AutoRotator"; }
		void Update(const float & deltaTime) override;
		/// <summary> Sets the axis to rotate around, and the rotation amount, in degrees per seccond. </summary>
		/// <param name="axis">The axis.</param>
		/// <param name="alpha">The rotation amount, in degrees per seccond.</param>
		void SetValues(const glm::vec3 & axis, const float & alpha);
		/// <summary> Gets the rotation axis. </summary>
		/// <returns>the rotation axis</returns>
		const glm::vec3 GetAxis() const;
		/// <summary> Gets rotation amount. </summary>
		/// <returns>the rotation amount</returns>
		const float GetAlpha() const;
	private:
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
		float alpha = 90.0f;
	};
}
#endif // !B00289996B00227422_AUTO_ROTATE_H