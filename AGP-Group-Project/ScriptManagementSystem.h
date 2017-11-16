#pragma once
#ifndef B00289996_SCRIPT_MAMAGEMENT_SYSTEM_H
#define B00289996_SCRIPT_MAMAGEMENT_SYSTEM_H
#include <vector>
#include <memory>
namespace B00289996 {
	class ScriptableComponent;
	class GameObject;
	class ScriptManagementSystem {
	public:
		ScriptManagementSystem();
		~ScriptManagementSystem();
		void UnloadScripts();
		void LoadScripts(std::vector<std::shared_ptr<GameObject>> & gameObjects);
		void FixedUpdate(const float & fixedDeltaTime);
		void Update(const float & deltaTime);
		void Render();
		void LateUpdate();
	private:
		std::vector<std::shared_ptr<ScriptableComponent>> updatable;
		std::vector<std::shared_ptr<ScriptableComponent>> fixedUpdatable;
		std::vector<std::shared_ptr<ScriptableComponent>> lateUpdatable;
		std::vector<std::shared_ptr<ScriptableComponent>> renderable;
	};
}
#endif // !B00289996_SCRIPT_MAMAGEMENT_SYSTEM_H