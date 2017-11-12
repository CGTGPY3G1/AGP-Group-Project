#include "GameObjectManager.h"
#include "GameObject.h"
namespace B00289996 {

	GameObjectManager & GameObjectManager::GetInstance() {
		static GameObjectManager instance;
		return instance;
	}

	GameObjectManager::~GameObjectManager() {
	}

	std::weak_ptr<GameObject> GameObjectManager::CreateGameObject(const std::string & name) {
		std::shared_ptr<GameObject> g;
		static unsigned int nextID = 0;
		if(freeIDs.size() > 0) {
			g = std::make_shared<GameObject>(freeIDs.back(), name);
			freeIDs.pop_back();
		}
		else {
			nextID++;
			if(nextID == 0) nextID++;
			g = std::make_shared<GameObject>(nextID, name);
		}
		gameObjects.push_back(g);
		g->Init();
		return g;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjects() {
		return gameObjects;
	}

	std::weak_ptr<GameObject> GameObjectManager::GetGameObject(const unsigned int & id, const bool retrieveDeleted) {
		std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasGameObjectID(id));
		if(it != gameObjects.end()) return (*it);
		if(retrieveDeleted) {
			it = std::find_if(toDelete.begin(), toDelete.end(), HasGameObjectID(id));
			if(it != toDelete.end()) return (*it);
		}
		return std::weak_ptr<GameObject>();
	}

	std::weak_ptr<GameObject> GameObjectManager::GetGameObject(const std::string & name, const bool retrieveDeleted) {
		std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasName(name));
		if(it != gameObjects.end()) return (*it);
		if(retrieveDeleted) {
			it = std::find_if(toDelete.begin(), toDelete.end(), HasName(name));
			if(it != toDelete.end()) return (*it);
		}
		return std::weak_ptr<GameObject>();
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsWithName(const std::string & name) {
		std::vector<std::shared_ptr<GameObject>> toReturn = std::vector<std::shared_ptr<GameObject>>();
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)	if((*it)->name.compare(name) == 0) toReturn.push_back(*it);
		return toReturn;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsWithAnyOfComponents(const unsigned int & componentMask) {
		std::vector<std::shared_ptr<GameObject>> toReturn = std::vector<std::shared_ptr<GameObject>>();
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)	if((*it)->HasAnyOfComponents(componentMask)) toReturn.push_back(*it);
		return toReturn;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsWithComponent(const unsigned int & componentMask) {
		std::vector<std::shared_ptr<GameObject>> toReturn = std::vector<std::shared_ptr<GameObject>>();
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)	if((*it)->HasComponents(componentMask)) toReturn.push_back(*it);
		return toReturn;
	}

	void GameObjectManager::DeleteGameObject(const unsigned int & id) {
		std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), HasGameObjectID(id));
		if(it != gameObjects.end()) {
			toDelete.push_back(*it);
			gameObjects.erase(it);
		}
	}

	GameObjectManager::GameObjectManager() {
	}

	void GameObjectManager::RemoveDeleted() {
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = toDelete.begin(); it != toDelete.end(); ++it) freeIDs.push_back((*it)->objectID);
		toDelete.clear();
	}

}
