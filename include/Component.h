#pragma once
#ifndef B00289996B00227422_COMPONENT_H
#define B00289996B00227422_COMPONENT_H

#include <memory>
#include <vector>
#include <string>
namespace B00289996B00227422 {
	enum ComponentType : unsigned int {
		COMPONENT_NULL = 0,
		COMPONENT_TRANSFORM = 1 << 0,
		COMPONENT_RIGID_BODY = 1 << 1,
		COMPONENT_MESH_RENDERER = 1 << 2,
		COMPONENT_CAMERA = 1 << 3,
		COMPONENT_LIGHT = 1 << 4,
		COMPONENT_PARTICLE_EMITTER = 1 << 5,
		COMPONENT_AUTO_ROTATOR = 1 << 6,
		COMPONENT_LIGHT_FLICKER = 1 << 7
		/*COMPONENT_ATTACKER_IDENTITY_SCRIPT = 1 << 7,
		COMPONENT_CHARACTER_SCRIPT = 1 << 8,
		COMPONENT_TILE_MAPPER = 1 << 10,
		COMPONENT_VEHICLE_CONTROLLER = 1 << 11,
		COMPONENT_BULLET_SCRIPT = 1 << 12,
		COMPONENT_PLAYER_SCRIPT = 1 << 13,
		COMPONENT_AUDIO_SOURCE = 1 << 14,
		COMPONENT_HEALTH_SCRIPT = 1 << 15,
		COMPONENT_DEATH_TIMER = 1 << 16,
		COMPONENT_SPRITE_ANIMATOR = 1 << 17,
		COMPONENT_BLOOD_SPLATTER_SCRIPT = 1 << 18,
		COMPONENT_WEAPON_CACHE = 1 << 19,
		COMPONENT_DAMAGE_SCRIPT = 1 << 20,
		COMPONENT_GRENADE_SCRIPT = 1 << 21,
		COMPONENT_SELF_DESTRUCTING_ANIM_SCRIPT = 1 << 22*/
	};

#define NUMBER_OF_COMPONENTS = 20

	class IDManager {
	public:
		~IDManager() {}
		static unsigned int GetNewCompID();
		static void RetireID(const unsigned int & id);
	private:
		static unsigned int compCount;
		static std::vector<unsigned int> freeIds;
	};

	struct ComponentData;
	class GameObject;
	class ScriptableComponent;
	class Component : public std::enable_shared_from_this<Component> {
	public:
		Component();
		Component(std::weak_ptr<GameObject> gameObject);
		virtual ~Component();
		virtual const ComponentType Type() const = 0;
		virtual void SetEnabled(const bool &enabled);
		bool GetEnabled();
		void SetOwner(std::weak_ptr<GameObject> gameObject);
		unsigned int GetCompID();
		unsigned int GetGameObjectID();
		const virtual std::string GetName() const = 0;
		std::string GetTag();
		std::weak_ptr<GameObject> GetGameObject();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> AddComponent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponentInParent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::vector<std::weak_ptr<T>> GetComponents();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> bool ComponentExistsInParents();

		virtual void Start();
		virtual void End();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void Destroy();
		/*virtual void HandleMessage(const Message & message) {};*/
		bool operator < (const Component & other) {
			return ownerID < other.ownerID && compID < other.compID;
		}
		bool operator > (const Component & other) {
			return ownerID > other.ownerID && compID > other.compID;
		}
		bool operator == (const Component & other) {
			return ownerID == other.ownerID && compID == other.compID;
		}
	protected:
		std::weak_ptr<Component> GetWeak() { return shared_from_this(); }
		bool enabled = false;
		unsigned int ownerID, compID;
		void Init(const bool & enabled);
		std::weak_ptr<GameObject> gameObject;
		ComponentData * componentData;
	};

	template<typename T> std::weak_ptr<T> Component::AddComponent() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(g) return g->AddComponent<T>();
		return std::weak_ptr<T>();
	}

	template<typename T> std::weak_ptr<T> Component::GetComponent() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(g) return g->GetComponent<T>();
		return std::weak_ptr<T>();
	}

	template<typename T>
	std::weak_ptr<T> Component::GetComponentInParent() {
		return gameObject.lock()->GetComponentInParent<T>();
	}

	template<typename T> std::vector<std::weak_ptr<T>> Component::GetComponents() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(g) return g->GetComponents<T>();
		return std::vector<std::weak_ptr<T>>();
	}
	template<typename T>
	bool Component::ComponentExistsInParents() {
		return gameObject.lock()->ComponentExistsInParents<T>();
	}
}
#endif // !B00289996B00227422_COMPONENT_H


