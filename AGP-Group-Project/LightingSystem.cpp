#include "LightingSystem.h"
#include "Light.h"
#include "GameObject.h"
#include "Shader.h"
#include "Lights.h"
#include "Transform.h"
namespace B00289996 {
	LightingSystem::LightingSystem() : System() {
	}
	LightingSystem::~LightingSystem() {
	}

	void LightingSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
		ClearLights();
		const unsigned int mask = GetComponentMask();
		for(size_t i = 0; i < gameObjects.size(); i++) {
			if(gameObjects[i]->HasComponents(mask)) {
				std::shared_ptr<Light> light = gameObjects[i]->GetComponent<Light>().lock();
				std::shared_ptr<Transform> transform = gameObjects[i]->GetComponent<Transform>().lock();
				if(light) {
					LightType type = light->GetLightType();
					switch(type) {
					case LightType::DIRECTION_LIGHT:
						directionLight = light->GetLight<DirectionLight>();
						directionLight->direction = transform->GetForward();
						break;
					case LightType::POINT_LIGHT:
					{
						std::shared_ptr<PointLight> pointLight = light->GetLight<PointLight>();
						pointLight->position = transform->GetPosition();
						pointLights.push_back(pointLight);
					}
						break;
					case LightType::SPOT_LIGHT:
					{
						std::shared_ptr<SpotLight> spotLight = light->GetLight<SpotLight>();
						spotLight->position = transform->GetPosition();
						spotLight->direction = transform->GetForward();
						spotLights.push_back(spotLight);
					}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	const unsigned int LightingSystem::GetComponentMask() const {
		return COMPONENT_LIGHT | COMPONENT_TRANSFORM;
	}

	void LightingSystem::UpdateShader(std::shared_ptr<ShaderProgram> shader) {
		if(directionLight) {
			shader->SetUniform<glm::vec3>("directionLightDirection", directionLight->direction);
			shader->SetUniform<glm::vec3>("directionLight.ambient", directionLight->ambient);
			shader->SetUniform<glm::vec3>("directionLight.diffuse", directionLight->diffuse);
			shader->SetUniform<glm::vec3>("directionLight.specular", directionLight->specular);
			shader->SetUniform<float>("directionLight.intensity", directionLight->intensity);
		}
		
		const unsigned int noOfPointLights = pointLights.size();
		shader->SetUniform<int>("numberOfPointLights", noOfPointLights);
		for(unsigned int i = 0; i < noOfPointLights; i++) {
			shader->SetUniform<glm::vec3>(("pointLightPositions[" + std::to_string(i) + "]").c_str(), pointLights[i]->position);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].ambient").c_str(), pointLights[i]->ambient);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), pointLights[i]->diffuse);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights[i]->specular);
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].attenuation").c_str(), pointLights[i]->attenuation);
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].lightLength").c_str(), pointLights[i]->lightLength);
		}
		const unsigned int noOfSpotLights = spotLights.size();
		shader->SetUniform<int>("numberOfSpotLights", noOfSpotLights);
		for(unsigned int i = 0; i < noOfSpotLights; i++) {
			shader->SetUniform<glm::vec3>(("spotLightPositions[" + std::to_string(i) + "]").c_str(), spotLights[i]->position);
			shader->SetUniform<glm::vec3>(("spotLightDirections[" + std::to_string(i) + "]").c_str(), spotLights[i]->direction);
			shader->SetUniform<glm::vec3>(("spotLights[" + std::to_string(i) + "].ambient").c_str(), spotLights[i]->ambient);
			shader->SetUniform<glm::vec3>(("spotLights[" + std::to_string(i) + "].diffuse").c_str(), spotLights[i]->diffuse);
			shader->SetUniform<glm::vec3>(("spotLights[" + std::to_string(i) + "].specular").c_str(), spotLights[i]->specular);
			shader->SetUniform<float>(("spotLights[" + std::to_string(i) + "].attenuation").c_str(), spotLights[i]->attenuation);
			shader->SetUniform<float>(("spotLights[" + std::to_string(i) + "].cutOff").c_str(), spotLights[i]->cutOff);
			shader->SetUniform<float>(("spotLights[" + std::to_string(i) + "].outerCutOff").c_str(), spotLights[i]->outerCutOff);
			shader->SetUniform<float>(("spotLights[" + std::to_string(i) + "].intensity").c_str(), spotLights[i]->intensity);
			shader->SetUniform<float>(("spotLights[" + std::to_string(i) + "].lightLength").c_str(), spotLights[i]->lightLength);
		}
	}

	void LightingSystem::ClearLights() {
		pointLights.clear();
		spotLights.clear();
		if(directionLight) directionLight = std::shared_ptr<DirectionLight>();
	}

}
