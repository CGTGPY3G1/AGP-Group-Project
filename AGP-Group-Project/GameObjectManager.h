#pragma once
#ifndef B00289996B00227422_GAMEOBJECT_MANAGER_H
#define B00289996B00227422_GAMEOBJECT_MANAGER_H
#include <memory>
#include <algorithm>
#include <vector>

namespace B00289996B00227422 {
	class GameObject;
	class GameObjectManager {
	public:
		static GameObjectManager & GetInstance();
		~GameObjectManager();
		std::weak_ptr<GameObject> CreateGameObject(const std::string & name = "New GameObject");
		std::vector<std::shared_ptr<GameObject>> GetGameObjects();
		std::weak_ptr<GameObject> GetGameObject(const unsigned int & id, const bool retrieveDeleted = false);
		std::weak_ptr<GameObject> GetGameObject(const std::string & name, const bool retrieveDeleted = false);
		std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithName(const std::string & name);
		std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithAnyOfComponents(const unsigned int & componentMask);
		std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithComponent(const unsigned int & componentMask);
		void DeleteGameObject(const unsigned int & id);
	protected:
		GameObjectManager();
		std::vector<unsigned int> freeIDs = std::vector<unsigned int>();
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		void RemoveDeleted();
		std::vector<std::shared_ptr<GameObject>> toDelete;

	};
	
}
#endif // !B00289996B00227422_GAMEOBJECT_MANAGER_H

