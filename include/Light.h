#pragma once
#ifndef B00289996B00227422_LIGHT_H
#define B00289996B00227422_LIGHT_H
#include "Component.h"
#include <glm\mat4x4.hpp>
#include <memory>
#include "Lights.h"
namespace B00289996B00227422 {
	enum LightType{
		DIRECTION_LIGHT,
		POINT_LIGHT
	};
	struct BaseLight;
	class Light : public Component {
	public:
		/// <summary>Initializes a new instance of the <see cref="Light"/> class. </summary>
		/// <param name="g">The g.</param>
		Light(const std::weak_ptr<GameObject> & g);
		/// <summary>Finalizes an instance of the <see cref="Light"/> class.</summary>
		~Light();
		const ComponentType Type() const override { return COMPONENT_LIGHT; }
		const std::string GetName() const override { return "Light"; }
		/// <summary> Gets the type of the light (Point/Directional).</summary>
		/// <returns>the light type</returns>
		const LightType GetLightType() const;
		/// <summary>Sets the type of the light. </summary>
		/// <param name="newType">The new type.</param>
		void SetLightType(const LightType & newType);
		/// <summary> Template for Getting different light types (partial specializations for directional Lights and point lights) </summary>
		/// <returns>the type of light reuested (assuming the Light is of that type)</returns>
		template <typename T> std::shared_ptr<T> GetLight() { return std::shared_ptr<T>(); }
		// templaye specialization for directional lights
		template <> std::shared_ptr<DirectionLight> Light::GetLight<DirectionLight>() {
			if (lightType == DIRECTION_LIGHT) return std::static_pointer_cast<DirectionLight> (light);
			return std::shared_ptr<DirectionLight>();
		}
		// templaye specialization for point lights
		template <>std::shared_ptr<PointLight> Light::GetLight<PointLight>() {
			if (lightType == POINT_LIGHT) return std::static_pointer_cast<PointLight> (light);
			return std::shared_ptr<PointLight>();
		}
	private:
		std::shared_ptr<BaseLight> light;
		LightType lightType;
	};
	
}

#endif // !B00289996B00227422_LIGHT_H