#pragma once
#ifndef B00289996_COLLISION_H
#define B00289996_COLLISION_H

#include "GameObject.h"
#include <glm\vec3.hpp>
namespace B00289996 {
	/// <summary>
	/// Data from a collision
	/// </summary>
	struct Collision {
		glm::vec3 normal; // collision normal
		glm::vec3 point; // closest contact point
		float distance; // collision depth
		std::weak_ptr<GameObject> gameObject; // colliding gameobject
	};

	/// <summary>
	/// Defines a pair of colliding objects
	/// </summary>
	struct GameObjectsCache {
		std::weak_ptr<GameObject> gameObject1, gameObject2;
	};

	/// <summary>
	/// Data from Raycast Intersection
	/// </summary>
	struct RayCastHit {
		/// <summary>
		/// Initializes a new instance of the <see cref="RayCastHit"/> struct.
		/// </summary>
		/// <param name="wasHit">did the ray hit anything.</param>
		/// <param name="norm">The collision normal.</param>
		/// <param name="hitPoint">The hit point.</param>
		/// <param name="distanceFromStart">The distance from the start of the ray.</param>
		/// <param name="go">The colliding object.</param>
		RayCastHit(const bool & wasHit = false, glm::vec3 norm = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 hitPoint = glm::vec3(0.0f, 0.0f, 0.0f), float distanceFromStart = 0.0f, std::weak_ptr<GameObject> go = std::weak_ptr<GameObject>())
			: hit(wasHit), normal(norm), point(hitPoint), distance(distanceFromStart), gameObject(go) {
		}
		const bool hit;
		const glm::vec3 normal;
		const glm::vec3 point;
		const float distance;
		const std::weak_ptr<GameObject> gameObject;
	};
}
#endif // !B00289996_COLLISION_H
