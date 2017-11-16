#pragma once
#ifndef B00289996_SCRIPTABLE_COMPONENT_H
#define B00289996_SCRIPTABLE_COMPONENT_H
#include "Component.h"

namespace B00289996 {
	struct Collision;
	class ScriptableComponent : public Component {
	public:
		ScriptableComponent();
		ScriptableComponent(std::weak_ptr<GameObject> gameObject);
		virtual ~ScriptableComponent() {}
		virtual void Update(const float & deltaTime) {}
		virtual void FixedUpdate(const float & fixedDeltaTime) {}
		virtual void Render() {}
		virtual void LateUpdate() {}
		void OnCollisionEnter(const Collision & collision) {}
		void OnCollisionExit(const Collision & collision) {}
		//virtual int GetSortOrder() = 0;
		/*virtual void OnCollisionEnter(const CollisionData & data) {}
		virtual void OnCollisionStay(const CollisionData & data) {}
		virtual void OnCollisionExit(const CollisionData & data) {}
		virtual void OnSensorEnter(const std::weak_ptr<Collider> & collider) {}
		virtual void OnSensorExit(const std::weak_ptr<Collider> & collider) {}*/
	protected:
		std::weak_ptr<ScriptableComponent> GetWeak() { return std::static_pointer_cast<ScriptableComponent>(shared_from_this()); }
	};

}
#endif // !B00289996_SCRIPTABLE_COMPONENT_H
