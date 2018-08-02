#ifndef B00289996B00227422_BOUNDS_H
#define B00289996B00227422_BOUNDS_H
#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>
namespace B00289996B00227422 {
	// Axis-Aligned bounding box
	struct AABB {
		AABB(const glm::vec3 & minimum = glm::vec3(0.0f), const glm::vec3 & maximum = glm::vec3(0.0f));
		glm::vec3 min, max;
	};

	// (Object) Oriented bounding box
	struct OBB {
		OBB(const glm::vec3 & position = glm::vec3(0.0f), const glm::vec3 & halfSize = glm::vec3(0.0f), const glm::quat & direction = glm::quat());
		OBB(const AABB & aabb);
		glm::vec3 centre, extents;
		glm::quat rotation;
	};

	struct Sphere {
		Sphere(const glm::vec3 & position = glm::vec3(0.0f), const float & radius = 0.0f);
		glm::vec3 centre;
		float radius;
	};
}
#endif // !B00289996B00227422_BOUNDS_H