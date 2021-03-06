#pragma once
#ifndef B00289996B00227422_COMPONENT_MANAGER_H
#define B00289996B00227422_COMPONENT_MANAGER_H
#include <memory>
#include <vector>
namespace B00289996B00227422 {
	class GameObject;
	class Component;
	class ScriptableComponent;
	struct Collision;
	class ComponentManager {
	public:
		ComponentManager();
		ComponentManager(std::weak_ptr<GameObject> gameObject);
		~ComponentManager();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponentInParent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::vector<std::weak_ptr<T>> GetComponents();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> AddComponent();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> bool ComponentExistsInParents();
		template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> void RemoveComponent(const unsigned int & id);

		std::vector<std::shared_ptr<ScriptableComponent>> GetScriptableComponents();
		void Start();
		void Update(const float & deltaTime);
		void FixedUpdate(const float & fixedDeltaTime);
		void LateUpdate();
		void SetEnabled(const bool & enabled);
		void OnCollisionEnter(const Collision & collision);
		void OnCollisionExit(const Collision & collision);
	private:
		int ownerID = 0;
		std::vector<std::shared_ptr<Component>> components;
		std::vector<std::shared_ptr<ScriptableComponent>> scriptableComponents;
		std::weak_ptr<GameObject> gameObject;
	};

	template<typename T> std::weak_ptr<T> ComponentManager::AddComponent() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		std::shared_ptr<T> t = std::make_shared<T>(gameObject);
		if(TypeInfo::IsScriptable<T>()) {
			std::shared_ptr<ScriptableComponent> sc = std::dynamic_pointer_cast<ScriptableComponent>(t);
			scriptableComponents.push_back(sc);
		}
		else components.push_back(t);
		g->IterateComponentCount<T>(true);
		return t;
	}

	template<typename T>
	std::weak_ptr<T> ComponentManager::GetComponent() {
		ComponentType type = TypeInfo::GetTypeID<T>();
		if(TypeInfo::IsScriptable<T>()) {
			for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
				if(type == (*i)->Type()) {
					std::weak_ptr<T> toReturn = std::dynamic_pointer_cast<T>(*i);
					return toReturn.expired() ? std::weak_ptr<T>() : toReturn;
				}
			}
		}
		else {
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(type == (*i)->Type()) {
					return std::static_pointer_cast<T>(*i);
				}
			}
		}
		return std::weak_ptr<T>();
	}
	
	template<typename T>
	std::weak_ptr<T> ComponentManager::GetComponentInParent() {
		std::shared_ptr<Transform2D> parent;
		ComponentType transformType = TypeInfo::GetTypeID<Transform2D>();
		for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
			if(transformType == (*i)->Type()) {
				parent = std::static_pointer_cast<Transform2D>(*i)->GetParent().lock();
				break;
			}
		}
		while(parent) {
			std::weak_ptr<T> comp = parent->GetComponent<T>();
			if(!comp.expired()) return comp;
			parent = parent->GetParent().lock();
		}
		return std::weak_ptr<T>();
	}

	template<typename T>
	std::vector<std::weak_ptr<T>> ComponentManager::GetComponents() {
		std::vector<std::weak_ptr<T>> toReturn = std::vector<std::weak_ptr<T>>();
		ComponentType type = TypeInfo::GetTypeID<T>();
		
		if (TypeInfo::IsScriptable<T>()) {
			for (std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
				if (type == (*i)->Type()) toReturn.push_back(std::dynamic_pointer_cast<T>(*i));
			}
		}
		else {
			for (std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if (type == (*i)->Type()) toReturn.push_back(std::static_pointer_cast<T>(*i));
			}
		}
		return toReturn;
	}

	template<typename T>
	bool ComponentManager::ComponentExistsInParents() {
		std::shared_ptr<Transform2D> parent;
		ComponentType transformType = TypeInfo::GetTypeID<Transform2D>();
		for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
			if(transformType == (*i)->Type()) {
				parent = std::static_pointer_cast<Transform2D>(*i)->GetParent().lock();
				break;
			}
		}
		bool found = false;
		while(parent && !found) {
			found = parent->GetGameObject().lock()->HasComponent<T>();
			parent = parent->GetParent().lock();
		}
		return found;
	}
	template<typename T>
	inline void ComponentManager::RemoveComponent(const unsigned int & id) {
		ComponentType type = TypeInfo::GetTypeID<T>();
		if(TypeInfo::IsScriptable<T>()) {
			for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
				if(type == (*i)->Type() && id == (*i)->GetCompID()) {
					scriptableComponents.erase(i);
					break;
				};
			}
		}
		else {
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(type == (*i)->Type() && id == (*i)->GetCompID()) {
					components.erase(i);
					break;
				};
			}
		}
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(g->IterateComponentCount<T>(false) == 0) g->componentMask &= ~TypeInfo::GetTypeID<T>();
	}
}

#endif // !B00289996B00227422_COMPONENT_MANAGER_H