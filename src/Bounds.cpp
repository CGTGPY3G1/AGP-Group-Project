#include "Bounds.h"
namespace B00289996B00227422 {

	AABB::AABB(const glm::vec3 & minimum, const glm::vec3 & maximum) : min(minimum), max(maximum) {
	}

	OBB::OBB(const glm::vec3 & position, const glm::vec3 & halfSize, const glm::quat & direction) : centre(position), extents(halfSize), rotation(direction) {
	}

	OBB::OBB(const AABB & aabb) {
		extents = (aabb.max - aabb.min) * 0.5f;
		centre = aabb.min + extents;
		rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	Sphere::Sphere(const glm::vec3 & position, const float & rad) : centre(position), radius(rad) {
	}

}
