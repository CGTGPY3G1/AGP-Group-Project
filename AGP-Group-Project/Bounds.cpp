#include "Bounds.h"
namespace B00289996 {

	AABB::AABB(const glm::vec3 & minimum, const glm::vec3 & maximum) : min(minimum), max(maximum) {
	}

	OBB::OBB(const glm::vec3 & position, const glm::vec3 & halfSize, const glm::quat & direction) : centre(position), halfExtents(halfSize), rotation(direction) {
	}

	OBB::OBB(const AABB & aabb) {
		centre = (aabb.min + aabb.max) * 0.5f;
		halfExtents = (aabb.max - aabb.min) * 0.5f;
		rotation = glm::quat();
	}

	Sphere::Sphere(const glm::vec3 & position, const float & rad) : centre(position), radius(rad) {
	}

}
