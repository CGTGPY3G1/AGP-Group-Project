#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"
#include "TypeConverter.h"
#include "PhysicsSystem.h"
#include "GameObjectData.h"
#include <iostream>

namespace B00289996 {

	RigidBody::RigidBody(const std::weak_ptr<GameObject> & g) : Component(g), body(nullptr) {
		gameObjectData = new GameObjectData();
		gameObjectData->gameObject = g;
	}

	RigidBody::~RigidBody() {

	}

	void RigidBody::Init(const bool & isDynamic, const float & angularDampening, const float & linearDampening) {
		if(IsInitialised()) {
			std::cout << "RigidBody " + std::to_string(compID) + " already initialised!" << std::endl;
			return;
		}
		else {
			PhysicsSystem::GetInstance().AttachRigidBody(std::static_pointer_cast<RigidBody>(shared_from_this()), isDynamic);
			body->setDamping(linearDampening, angularDampening);
		}
	}

	void RigidBody::AddForce(const glm::vec3 & force, const bool & impulse) {
		if(body) {
			if(impulse) body->applyCentralImpulse(TypeConverter::ConvertToBulletVector3(force));
			else body->applyCentralForce(TypeConverter::ConvertToBulletVector3(force));
		}
	}

	void RigidBody::AddForce(const glm::vec3 & force, const glm::vec3 & offset, const bool & impulse) {
		if(body) {
			if(impulse) body->applyImpulse(TypeConverter::ConvertToBulletVector3(force), TypeConverter::ConvertToBulletVector3(offset));
			else body->applyForce(TypeConverter::ConvertToBulletVector3(force), TypeConverter::ConvertToBulletVector3(offset));
		}
	}

	void RigidBody::AddTorque(const glm::vec3 & torque, const bool & impulse) {
		if(body) {
			if(impulse) body->applyTorqueImpulse(TypeConverter::ConvertToBulletVector3(torque));
			else body->applyTorque(TypeConverter::ConvertToBulletVector3(torque));
		}
	}

	void RigidBody::SetLinearVelocity(const glm::vec3 & velocity) {
		if(body) body->setLinearVelocity(TypeConverter::ConvertToBulletVector3(velocity));
	}

	const glm::vec3 RigidBody::GetLinearVelocity() {
		return body ? TypeConverter::ConvertToGLMVector3(body->getLinearVelocity()) : glm::vec3();
	}

	void RigidBody::SetAngularVelocity(const glm::vec3 & velocity) {
		if(body) body->setAngularVelocity(TypeConverter::ConvertToBulletVector3(velocity));
	}

	const glm::vec3 RigidBody::GetAngularVelocity() {
		return body ? TypeConverter::ConvertToGLMVector3(body->getAngularVelocity()) : glm::vec3();
	}

	void RigidBody::SetPosition(const glm::vec3 & position) {
		if(body) {
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			transform.setOrigin(TypeConverter::ConvertToBulletVector3(position));
			body->proceedToTransform(transform);
			body->activate(true);
		}
	}

	const glm::vec3 RigidBody::GetPosition() {
		if(body) {
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			return TypeConverter::ConvertToGLMVector3(transform.getOrigin());
		}
		else if(gameObject.use_count() > 0) {
			return gameObject.lock()->GetComponent<Transform>().lock()->GetPosition();
		}
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void RigidBody::SetRotation(const glm::quat & rotationn) {
		if(body) {
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			transform.setRotation(TypeConverter::ConvertToBulletQuat(rotationn));
			body->proceedToTransform(transform);
			body->activate(true);
		}
	}

	const glm::quat RigidBody::GetRotation() {
		if(body) {
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			return TypeConverter::ConvertToGLMQuat(transform.getRotation());
		}
		else if(gameObject.use_count() > 0) {
			return gameObject.lock()->GetComponent<Transform>().lock()->GetPosition();
		}
		return glm::quat();
	}

	void RigidBody::SetAngularDampening(const float & angularDampening) {
		if(body) body->setDamping(body->getLinearDamping(), angularDampening);
	}

	const float RigidBody::GetAngularDampening() {
		return (body != nullptr ? body->getAngularDamping() : 1.0f);
	}

	void RigidBody::SetLinearDampening(const float & linearDampening) {
		if(body) body->setDamping(linearDampening, body->getAngularDamping());
	}

	const float RigidBody::GetLinearDampening() {
		return IsInitialised() ? body->getLinearDamping() : 1.0f;
	}

	void RigidBody::SetLocalGravity(const glm::vec3 & gravity) {
		if(IsInitialised()) body->setGravity(TypeConverter::ConvertToBulletVector3(gravity));
	}

	const glm::vec3 RigidBody::GetLocalGravity() {
		return IsInitialised() ? TypeConverter::ConvertToGLMVector3(body->getGravity()) : glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void RigidBody::SetFriction(const float & friction) {
		if(body) {
			body->setFriction(friction);
			body->setRollingFriction(friction * 0.1f);
		}
	}

	const float RigidBody::GetFriction() {
		return IsInitialised() ? body->getFriction() : 0.0f;
	}

	void RigidBody::SetRestitition(const float & restitution) {
		if(body) body->setRestitution(restitution);
	}

	const float RigidBody::GetRestitition() {
		return IsInitialised() ? body->getRestitution() : 0.0f;;
	}

	const float RigidBody::GetMass() {
		if(body) return 1.0f / body->getInvMass();
		return 0.0f;
	}

	const bool RigidBody::IsTrigger() const {
		return body != nullptr ? ((body->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == btCollisionObject::CF_NO_CONTACT_RESPONSE) ? true : false : false;
	}

	void RigidBody::SetTrigger(const bool & val) {
		if(body) {
			if(val) body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			else if((body->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == btCollisionObject::CF_NO_CONTACT_RESPONSE) body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}

	void RigidBody::SetCollisionGroup(const int & group) {
		collisionGroup = group;
		if(body) body->setCollisionFlags(group);
	}

	const int RigidBody::GetCollisionGroup() {
		return collisionGroup;
	}

	const bool RigidBody::IsInitialised() {
		return body != nullptr;
	}

	const bool RigidBody::IsKinematic() {
		return body ? body->isKinematicObject() : false;
	}

	const bool RigidBody::IsStatic() {
		return body ? body->isStaticObject() : false;
	}

	const bool RigidBody::IsDynamic() {
		return body ? !body->isStaticOrKinematicObject() : false;
	}

	void RigidBody::AttachSphere(const glm::vec3 & offset, const float & radius, const float & mass) {
		PhysicsSystem::GetInstance().AddSphere(std::static_pointer_cast<RigidBody>(shared_from_this()), offset, radius, mass);
	}

	void RigidBody::AttachBox(const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {
		PhysicsSystem::GetInstance().AddBox(std::static_pointer_cast<RigidBody>(shared_from_this()), offset, dimensions, mass);
	}

	void RigidBody::AttachCapsule(const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		PhysicsSystem::GetInstance().AddCapsule(std::static_pointer_cast<RigidBody>(shared_from_this()), offset, height, radius, mass);
	}

	void RigidBody::AttachCylinder(const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {
		PhysicsSystem::GetInstance().AddCylinder(std::static_pointer_cast<RigidBody>(shared_from_this()), offset, dimensions, mass);
	}

	void RigidBody::AttachCharacterController(const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		PhysicsSystem::GetInstance().AddCharacterController(std::static_pointer_cast<RigidBody>(shared_from_this()), offset, height, radius, mass);
	}

	void RigidBody::Destroy() {
		if(body) PhysicsSystem::GetInstance().RemoveBody(body);
		body = nullptr;
	}

	void RigidBody::OnEnable() {
		if (body) {
			PhysicsSystem::GetInstance().AddBody(body);
		}
	}

	void RigidBody::OnDisable() {
		if (body) {
			PhysicsSystem::GetInstance().RemoveBody(body);
		}
	}

	void RigidBody::SetBody(btRigidBody * body) {
		if(!gameObjectData) {
			gameObjectData = new GameObjectData();
			gameObjectData->gameObject = gameObject.lock();
		}
		this->body = body;
	}

	GameObjectData * RigidBody::GetData() {
		if(!gameObjectData) {
			gameObjectData = new GameObjectData();
			gameObjectData->gameObject = gameObject.lock();
		}
		return gameObjectData;
	}
}