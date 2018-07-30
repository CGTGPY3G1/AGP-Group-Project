#pragma once
#ifndef B00289996B00227422_LIGHTS_H
#define B00289996B00227422_LIGHTS_H
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <GL\glew.h>
namespace B00289996B00227422 {
	/// <summary>
	/// Abstraction used to store/access derived lights
	/// </summary>
	struct BaseLight {
		glm::vec3 position;
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	};

	struct DirectionLight : BaseLight {
		glm::vec3 direction;
		float intensity;
	};

	struct PointLight : BaseLight {
		float attenuation;
		float lightLength;
	};

}

#endif //!B00289996B00227422_LIGHTS_H
