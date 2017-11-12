#ifndef B00289996_TYPE_INFO_H
#define B00289996_TYPE_INFO_H

#include "ComponentHeaders.h"
namespace B00289996 {
	class TypeInfo {
	public:
		template <typename T> const static ComponentType GetTypeID() { return ComponentType::COMPONENT_NULL; };
		template<> const static ComponentType GetTypeID<Transform>() { return ComponentType::COMPONENT_TRANSFORM; }
		template<> const static ComponentType GetTypeID<MeshRenderer>() { return ComponentType::COMPONENT_MESH_RENDERER; }
		template<> const static ComponentType GetTypeID<RigidBody>() { return ComponentType::COMPONENT_RIGID_BODY; }
		template<> const static ComponentType GetTypeID<Camera>() { return ComponentType::COMPONENT_CAMERA; }
		template<> const static ComponentType GetTypeID<Light>() { return ComponentType::COMPONENT_LIGHT; }
		template<> const static ComponentType GetTypeID<AutoRotator>() { return ComponentType::COMPONENT_AUTO_ROTATOR; }
		template<> const static ComponentType GetTypeID<ParticleEmitter>() { return ComponentType::COMPONENT_PARTICLE_EMITTER; }
		template<> const static ComponentType GetTypeID<LightFlicker>() { return ComponentType::COMPONENT_LIGHT_FLICKER; }

		template <typename T> const static bool AllowMultiple() { return false; };
		template<> const static bool AllowMultiple<AutoRotator>() { return true; }
		template<> const static bool AllowMultiple<ParticleEmitter>() { return true; }

		template <typename T> const static bool IsScriptable() { return false; };
		template<> const static bool IsScriptable<AutoRotator>() { return true; }
		template<> const static bool IsScriptable<ParticleEmitter>() { return true; }
		template<> const static bool IsScriptable<LightFlicker>() { return true; }

		template <typename T> const static int ScriptSortOrder() { return 100000; };

		template <typename T> const static bool IsCollider() { return false; };

		const static bool IsUpdatable(const unsigned int & type) {
			static const int mask = COMPONENT_AUTO_ROTATOR | COMPONENT_PARTICLE_EMITTER | COMPONENT_LIGHT_FLICKER;
			return ((mask & type) == type);
		};

		const static bool IsFixedUpdatable(const ComponentType & type) {
			/*static const int mask = COMPONENT_CHARACTER_SCRIPT |
				COMPONENT_PLAYER_SCRIPT | COMPONENT_VEHICLE_CONTROLLER;
			return ((mask & type) == type);*/
			return false;
		};

		//const static bool IsLateUpdatable(const ComponentType & type) {
		//	/*static const int colliderMask = COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT;
		//	return (colliderMask & type) == type;*/
		//	return false;
		//};

		//const static bool IsRenderable(const ComponentType & type) {
		//	static const int mask = COMPONENT_VEHICLE_CONTROLLER;
		//	return ((mask & type) == type);
		//};

		//const static bool IsCollider(const ComponentType & type) {
		//	static const int mask = COMPONENT_BOX_COLLIDER_2D | COMPONENT_CIRCLE_COLLIDER | COMPONENT_POLYGON_COLLIDER_2D;
		//	return ((mask & type) == type);
		//};
	};
}
#endif // !B00289996_TYPE_INFO_H