#pragma once
#ifndef B00289996B00227422_SYSTEM_H
#define B00289996B00227422_SYSTEM_H
#include<memory>
#include <vector>
namespace B00289996B00227422 {
	class GameObject;
	class System : public std::enable_shared_from_this<System> {
	public:
		virtual ~System() {}
		virtual void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) = 0;
		virtual const unsigned int GetComponentMask() const = 0;
	};
}
#endif // !B00289996B00227422_SYSTEM_H
