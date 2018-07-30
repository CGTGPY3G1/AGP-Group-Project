#include "Collider.h"
#include "GameObject.h"
#include "RigidBody.h"
namespace B00289996B00227422 {
	Collider::Collider() : Component() {
	}

	Collider::Collider(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(!g->HasComponent<RigidBody>()) {
			std::shared_ptr<RigidBody> r = g->AddComponent<RigidBody>().lock();
			r->Init();
		}
	}

	Collider::~Collider() {
	}

	/*AABB Collider::GetAABB() {
		b2AABB aabb;
		fixture->GetShape()->ComputeAABB(&aabb, GetComponent<RigidBody2D>().lock()->GetBody()->GetTransform(), 0);
		return TypeConversion::ConvertToAABB(aabb);
	}*/

}
