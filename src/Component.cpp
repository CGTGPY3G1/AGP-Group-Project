#include "Component.h"
#include "GameObject.h"
namespace B00289996B00227422 {
	unsigned int IDManager::compCount = 0;
	std::vector<unsigned int> IDManager::freeIds = std::vector<unsigned int>();
	unsigned int IDManager::GetNewCompID() {
		if(freeIds.size() > 0) {
			const unsigned int toReturn = freeIds.back();
			freeIds.pop_back();
			return toReturn;
		}	
		++compCount;
		if(compCount == 0) ++compCount;
		return compCount;
	}

	void IDManager::RetireID(const unsigned int & id) {
		freeIds.push_back(id);
	}

	Component::Component() {
	}

	Component::Component(std::weak_ptr<GameObject> gameObject) {
		this->gameObject = gameObject;
		Init(true);
	}

	Component::~Component() {
		IDManager::RetireID(compID);
	}

	void Component::SetEnabled(const bool & enabled) {
		if(this->enabled != enabled) {
			this->enabled = enabled;
			if(enabled) OnEnable();
			else OnDisable();
		}
	}

	bool Component::GetEnabled() {
		return enabled;
	}

	void Component::SetOwner(std::weak_ptr<GameObject> gameObject) {
		this->gameObject = gameObject;
	}

	unsigned int Component::GetCompID() {
		return compID;
	}

	unsigned int Component::GetGameObjectID() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		return g->GetObjectID();
	}

	std::string Component::GetTag() {
		return gameObject.lock()->GetTag();
	}

	std::weak_ptr<GameObject> Component::GetGameObject() {
		return gameObject;
	}

	void Component::Start() {
	}

	void Component::End() {
	}

	void Component::OnEnable() {
	}

	void Component::OnDisable() {
	}

	void Component::Destroy() {

	}

	void Component::Init(const bool & enabled) {
		SetEnabled(enabled);
		compID = IDManager::GetNewCompID();
		if(gameObject.use_count() > 0) ownerID = gameObject.lock()->GetObjectID();
	}

}
