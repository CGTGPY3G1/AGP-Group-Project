#include "LightFlicker.h"
#include "Light.h"
#include "GameObject.h"
#include <glm\gtc\random.hpp>

namespace B00289996B00227422 {
	LightFlicker::LightFlicker() : ScriptableComponent() {
	}

	LightFlicker::LightFlicker(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
		min = glm::linearRand(minMin, maxMin);
		max = glm::linearRand(minMax, maxMax);
		current = glm::linearRand(min, max);
	}

	LightFlicker::~LightFlicker() {
	}

	void LightFlicker::Update(const float & deltaTime) {
		if(light.use_count() == 0) light = GetComponent<Light>();
		if(lightTransform.use_count() == 0) {
			lightTransform = GetComponent<Transform>();
			if(lightTransform.use_count() > 0) startPosition = lightTransform.lock()->GetPosition();
		}
		std::shared_ptr<Light> l = light.lock();
		if(l) {
			std::shared_ptr<PointLight> pl = l->GetLight<PointLight>();
			if(pl) {
				pl->attenuation = GetNewAttenuation(deltaTime);
			}
		}
		std::shared_ptr<Transform> t = lightTransform.lock();
		if(t) {
			t->SetPosition(startPosition + glm::vec3(0.0f, glm::linearRand(-0.005f, 0.005f), 0.0f));
		}
	}

	float LightFlicker::GetNewAttenuation(const float & deltaTime) {
		if(increasing) {
			current += speed * deltaTime;
			if(current >= max) {
				current = max;
				increasing = false;
				max = glm::linearRand(minMax, maxMax);
			}
		}
		else {
			current -= speed * deltaTime;
			if(current <= min) {
				current = min;
				increasing = true;
				min = glm::linearRand(minMin, maxMin);
			}
		}
		return current;
	}
}
