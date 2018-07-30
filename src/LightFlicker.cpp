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
		// if the light Component has not been retrieved, retrieve it
		if(light.use_count() == 0) light = GetComponent<Light>();
		if(lightTransform.use_count() == 0) {// if the lights transform component has not been retrieved, retrieve it
			lightTransform = GetComponent<Transform>();
			// if the light transfor exists, set the starting position using it's position in world space
 			if(lightTransform.use_count() > 0) startPosition = lightTransform.lock()->GetPosition();
		}
		std::shared_ptr<Light> l = light.lock();
		if(l) { // if a light has been attatched to the light script
			std::shared_ptr<PointLight> pl = l->GetLight<PointLight>();
			if(pl) { // and that light is a point light
				pl->attenuation = GetNewAttenuation(deltaTime); // change it's attenuation value
			}
		}
		// move the light to a pseudo-random position relevant to the starting position
		std::shared_ptr<Transform> t = lightTransform.lock();
		if(t) t->SetPosition(startPosition + glm::vec3(0.0f, glm::linearRand(-0.005f, 0.005f), 0.0f));
	}

	float LightFlicker::GetNewAttenuation(const float & deltaTime) {
		if(increasing) { // if the attenuation value is set to increase
			current += speed * deltaTime; //increase the current attenuation value
			if(current >= max) { // if the current value if higher than the max allowed value
				current = max;
				increasing = false;
				min = glm::linearRand(minMin, maxMin); // set a new psuedo-random minimum value
			}
		}
		else { // same as above but in reverse
			current -= speed * deltaTime;
			if(current <= min) {
				current = min;
				increasing = true;
				max = glm::linearRand(minMax, maxMax);// set a new psuedo-random maximum value
			}
		}
		return current;
	}
}
