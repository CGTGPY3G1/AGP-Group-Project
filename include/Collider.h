#pragma once
#ifndef B00289996B00227422_COLLIDER_H
#define B00289996B00227422_COLLIDER_H
#include "Component.h"
#include <btBulletCollisionCommon.h>
#include <glm\vec3.hpp>
namespace B00289996B00227422 {
	struct PhysicsMaterial {
		float friction = 1.0f, restitution = 1.0f;
	};
	struct AABB;
	struct ComponentData;
	class Collider : public Component {
		friend class PhysicsSystem;
	public:
		Collider();
		Collider(std::weak_ptr<GameObject> gameObject);
		virtual ~Collider();
		virtual glm::vec3 GetOffset() { return glm::vec3(); }
		virtual void SetOffset(const glm::vec3 & newOffset) {}
		//AABB GetAABB();
	protected:
		btCollisionShape * shape;
		float density, restitution, friction;
		bool isSensor;
	};
}
#endif // !B00289996B00227422_COLLIDER_H