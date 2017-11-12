#include "Light.h"

namespace B00289996 {
	Light::Light(const std::weak_ptr<GameObject>& g) : Component(g) {
		light = std::make_shared<DirectionLight>();
		lightType = DIRECTION_LIGHT;
	}

	Light::~Light() {
	}

	const LightType Light::GetLightType() const {
		return lightType;
	}

	void Light::SetLightType(const LightType & newType) {
		if(lightType != newType) {
			switch(newType) {
			case DIRECTION_LIGHT:
				lightType = newType;
				light = std::make_shared<DirectionLight>();
				break;
			case POINT_LIGHT:
				lightType = newType;
				light = std::make_shared<PointLight>();
				break;
			case SPOT_LIGHT:
				lightType = newType;
				light = std::make_shared<SpotLight>();
				break;
			default:
				break;
			}
		}
	}

	
}
