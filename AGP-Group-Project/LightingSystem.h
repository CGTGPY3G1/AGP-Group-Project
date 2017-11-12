#pragma once
#ifndef B00289996_LIGHTING_SYSTEM_H
#define B00289996_LIGHTING_SYSTEM_H

#include "System.h"

namespace B00289996 {
	struct SpotLight;
	struct DirectionLight;
	struct PointLight;
	class ShaderProgram;
	class LightingSystem : public System {
	public:
		LightingSystem();
		~LightingSystem();
		void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) override;
		const unsigned int GetComponentMask() const override;
		void UpdateShader(std::shared_ptr<ShaderProgram> shader);
		void ClearLights();
	private:
		std::vector<std::shared_ptr<PointLight>> pointLights;
		std::vector<std::shared_ptr<SpotLight>> spotLights;
		std::shared_ptr<DirectionLight> directionLight;
	};
}
#endif // !B00289996_LIGHTING_SYSTEM_H
