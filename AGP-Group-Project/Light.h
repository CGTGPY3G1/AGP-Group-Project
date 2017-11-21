#pragma once
#ifndef B00289996_LIGHT_H
#define B00289996_LIGHT_H
#include "Component.h"
#include <glm\mat4x4.hpp>
#include <memory>
#include "Lights.h"
namespace B00289996 {
	enum LightType{
		DIRECTION_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};
	struct BaseLight;
	class Light : public Component {
	public:
		Light(const std::weak_ptr<GameObject> & g);
		~Light();
		const ComponentType Type() const override { return COMPONENT_LIGHT; }
		const std::string GetName() const override { return "Light"; }
		const LightType GetLightType() const;
		void SetLightType(const LightType & newType);
		template <typename T> 
		std::shared_ptr<T> GetLight() { return std::shared_ptr<T>(); }
		template <>
		std::shared_ptr<DirectionLight> Light::GetLight<DirectionLight>() {
			if(lightType == DIRECTION_LIGHT) return std::static_pointer_cast<DirectionLight> (light);
			return std::shared_ptr<DirectionLight>();
		}

		template <>
		std::shared_ptr<PointLight> Light::GetLight<PointLight>() {
			if(lightType == POINT_LIGHT) return std::static_pointer_cast<PointLight> (light);
			return std::shared_ptr<PointLight>();
		}
	private:
		std::shared_ptr<BaseLight> light;
		LightType lightType;
	};
	
}

#endif // !B00289996_LIGHT_H