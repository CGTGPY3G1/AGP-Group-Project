#include "PhysicsSystem.h"
#include "Time.h"
#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "RigidBody.h"
#include "DebugDraw.h"
#include "GameObjectData.h"
#include "TypeConverter.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "Collision.h"
namespace B00289996B00227422 {
	PhysicsSystem & PhysicsSystem::GetInstance() {
		static PhysicsSystem instance;
		return instance;
	}

	PhysicsSystem::PhysicsSystem() {
		// Configure the Physics World
		collisionConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
		world->setGravity(TypeConverter::ConvertToBulletVector3(gravity));
		DebugDraw::GetInstance().setDebugMode(btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe);
		world->setDebugDrawer(&DebugDraw::GetInstance());
		// Set Up Collision Callbacks
		gContactProcessedCallback = (ContactProcessedCallback)BeginContactCallBack;
		gContactDestroyedCallback = (ContactDestroyedCallback)EndContactCallBack;
	}

	PhysicsSystem::~PhysicsSystem() {
		if (world) {
			btCollisionObjectArray objects = world->getCollisionObjectArray();
			for (size_t i = 0; i < objects.size(); i++) {
				btCollisionObject* obj = objects[i];
				GameObjectData * data = (GameObjectData*)obj->getUserPointer();

				world->removeCollisionObject(obj);
				delete obj;
				obj = nullptr;
				std::shared_ptr<GameObject> g = data->gameObject.lock();
				if (g) {
					std::shared_ptr<RigidBody> rb = g->GetComponent<RigidBody>().lock();
					if (rb && rb->body) {
						rb->body = nullptr;
					}
				}

			}
		}
		delete world;
		world = nullptr;
		delete dispatcher;
		dispatcher = nullptr;
		delete solver;
		solver = nullptr;
		delete broadphase;
		broadphase = nullptr;
	}

	void PhysicsSystem::Update(const float & deltaTime) {
		world->stepSimulation(deltaTime); // step the simulation
	}

	void PhysicsSystem::Draw() {
		if (drawDebug) {
			world->debugDrawWorld(); // Draw the Debud Drawer 
			DebugDraw::GetInstance().Render();
		}
	}

	void PhysicsSystem::AddSphere(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const float & radius, const float & mass) {

		if (rb) { // if the object has a RigidBody
				  // create the sphere

			btSphereShape* sphere = new btSphereShape(radius);
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(offset.x, offset.y, offset.z));
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			btCompoundShapeChild * children = shape->getChildList();
			int noOfChildren = shape->getNumChildShapes();
			for (int i = 0; i < noOfChildren; i++) {
				if (children[i].m_childShapeType == EMPTY_SHAPE_PROXYTYPE) {
					shape->removeChildShapeByIndex(i);
					break;
				}
			}

			shape->addChildShape(t, sphere);
			btVector3 inertia(0, 0, 0);
			noOfChildren = shape->getNumChildShapes();
			btScalar m = (rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f) + mass;

			if (m != 0.0f) {
				if (noOfChildren > 0) {
					std::vector<btScalar> masses;
					btScalar distributedMass = m / noOfChildren;
					for (int i = 0; i < noOfChildren; i++) {
						masses.push_back(distributedMass);
					}
					try {
						shape->calculatePrincipalAxisTransform(masses.data(), t, inertia);// if object has mass calculate inertia
					}
					catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
				}
			}
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
			rb->body->setMassProps(m, inertia);
			rb->body->updateInertiaTensor();
			rb->body->setActivationState(ACTIVE_TAG);
			sphere->setUserPointer(rb->GetData());
		}
		else std::cout << "Invalid RigidBody:  Cannot Attach Sphere" << std::endl;
	}

	void PhysicsSystem::AddBox(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {
		if (rb) { // if the object has a RigidBody
			btBoxShape* box = new btBoxShape(btVector3(dimensions.x * 0.5f, dimensions.y * 0.5f, dimensions.z * 0.5f));
			btTransform t;
			t.setIdentity();
			t.setOrigin(TypeConverter::ConvertToBulletVector3(offset));
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			btCompoundShapeChild * children = shape->getChildList();
			int noOfChildren = shape->getNumChildShapes();
			for (int i = 0; i < noOfChildren; i++) {
				if (children[i].m_childShapeType == EMPTY_SHAPE_PROXYTYPE) {
					shape->removeChildShape(children[i].m_childShape);
				}
			}
			shape->addChildShape(t, box);
			btVector3 inertia(0, 0, 0);
			noOfChildren = shape->getNumChildShapes();
			btScalar m = (rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f) + mass;

			if (m != 0.0f) {
				if (noOfChildren > 0) {
					std::vector<btScalar> masses;
					btScalar distributedMass = m / noOfChildren;
					for (int i = 0; i < noOfChildren; i++) {
						masses.push_back(distributedMass);
					}
					try {
						shape->calculatePrincipalAxisTransform(masses.data(), t, inertia);// if object has mass calculate inertia
					}
					catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
				}
			}
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
			rb->body->setMassProps(m, inertia);
			rb->body->updateInertiaTensor();
			rb->body->setActivationState(ACTIVE_TAG);
			box->setUserPointer(rb->GetData());
		}
		else std::cout << "Invalid RigidBody:  Cannot Attach Box" << std::endl;
	}

	void PhysicsSystem::AddCapsule(const std::shared_ptr<RigidBody>& rb, const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		if (rb) { // if the object has a RigidBody
			btCapsuleShape* capsule = new btCapsuleShape(radius, height);
			btTransform t;
			t.setIdentity();
			t.setOrigin(TypeConverter::ConvertToBulletVector3(offset));
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			btCompoundShapeChild * children = shape->getChildList();
			int noOfChildren = shape->getNumChildShapes();
			for (int i = 0; i < noOfChildren; i++) {
				if (children[i].m_childShapeType == EMPTY_SHAPE_PROXYTYPE) {
					shape->removeChildShapeByIndex(i);
					break;
				}
			}
			shape->addChildShape(t, capsule);
			btVector3 inertia(0, 0, 0);
			noOfChildren = shape->getNumChildShapes();
			btScalar m = (rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f) + mass;

			if (m != 0.0f) {
				if (noOfChildren > 0) {
					std::vector<btScalar> masses;
					btScalar distributedMass = m / noOfChildren;
					for (int i = 0; i < noOfChildren; i++) {
						masses.push_back(distributedMass);
					}
					try {
						shape->calculatePrincipalAxisTransform(masses.data(), t, inertia);// if object has mass calculate inertia
					}
					catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
					
				}
			}
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
			rb->body->setMassProps(m, inertia);
			rb->body->updateInertiaTensor();
			rb->body->setActivationState(ACTIVE_TAG);
			capsule->setUserPointer(rb->GetData());
		}
		else std::cout << "Invalid RigidBody:  Cannot Attach Capsule" << std::endl;
	}

	void PhysicsSystem::AddCylinder(const std::shared_ptr<RigidBody>& rb, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {

		if (rb) { // if the object has a RigidBody
			btCylinderShape* cylinder = new btCylinderShape(btVector3(dimensions.x * 0.5f, dimensions.y * 0.5f, dimensions.z * 0.5f));
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(offset.x, offset.y, offset.z));
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			btCompoundShapeChild * children = shape->getChildList();
			int noOfChildren = shape->getNumChildShapes();
			for (int i = 0; i < noOfChildren; i++) {
				if (children[i].m_childShapeType == EMPTY_SHAPE_PROXYTYPE) {
					shape->removeChildShapeByIndex(i);
					break;
				}
			}
			shape->addChildShape(t, cylinder);
			btVector3 inertia(0, 0, 0);
			noOfChildren = shape->getNumChildShapes();
			btScalar m = (rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f) + mass;

			if (m != 0.0f) {
				if (noOfChildren > 0) {
					std::vector<btScalar> masses;
					btScalar distributedMass = m / noOfChildren;
					for (int i = 0; i < noOfChildren; i++) {
						masses.push_back(distributedMass);
					}
					try {
						shape->calculatePrincipalAxisTransform(masses.data(), t, inertia);// if object has mass calculate inertia
					}
					catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
				}
			}
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematicematic
			rb->body->setMassProps(m, inertia);
			rb->body->updateInertiaTensor();
			rb->body->setActivationState(ACTIVE_TAG);
			cylinder->setUserPointer(rb->GetData());
		}
		else std::cout << "Invalid RigidBody:  Cannot Attach Cylinder" << std::endl;
	}

	void PhysicsSystem::AddCharacterController(const std::shared_ptr<RigidBody>& rb, const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		if (rb) {
			AddCapsule(rb, offset, height, radius, mass); // adds a capsule collider
			rb->body->setAngularFactor(btVector3(0, 1, 0)); // prevent the body from rotating on the X and Z axes
			rb->SetAngularDampening(0.98f); // Set a fairly high Angular Dampening factor
			rb->SetLinearDampening(0.9f); // and a fairly high Linear Dampening factor
			rb->SetFriction(1.0f); // and a large amount of friction
			rb->body->setActivationState(DISABLE_DEACTIVATION);
		}
		else std::cout << "Invalid RigidBody:  Cannot Attach Character Controller" << std::endl;
	}

	RayCastHit PhysicsSystem::Raycast(const glm::vec3 & from, const glm::vec3 & to, const int & filter) {
		const btVector3 start = TypeConverter::ConvertToBulletVector3(from), end = TypeConverter::ConvertToBulletVector3(to);
		btCollisionWorld::ClosestRayResultCallback rayCallback(start, end); // use a built in call back function to cast the ray 
		rayCallback.m_collisionFilterMask = filter; // define what it collides with
		world->rayTest(start, end, rayCallback);
		if (rayCallback.hasHit()) { // if the ray intersected an object
			GameObjectData* gameObjectData = (GameObjectData*)rayCallback.m_collisionObject->getUserPointer();
			if (gameObjectData) { // and the object is recognisable as a GameObject
								  // return the Intersection data
				std::weak_ptr<GameObject> g = std::dynamic_pointer_cast<GameObject>(gameObjectData->gameObject.lock());
				std::shared_ptr<GameObject> gameObject = g.lock();
				return RayCastHit(true, TypeConverter::ConvertToGLMVector3(rayCallback.m_hitNormalWorld), TypeConverter::ConvertToGLMVector3(rayCallback.m_hitPointWorld), rayCallback.m_closestHitFraction, gameObject);
			}
		}
		return RayCastHit();
	}

	std::vector<RayCastHit> PhysicsSystem::RaycastAll(const glm::vec3 & from, const glm::vec3 & to, const int & filter) {
		std::vector<RayCastHit> toReturn = std::vector<RayCastHit>(); // create an array of hits
		const btVector3 start = TypeConverter::ConvertToBulletVector3(from), end = TypeConverter::ConvertToBulletVector3(to);
		btCollisionWorld::AllHitsRayResultCallback rayCallback(start, end); // use a built in call back function to cast the ray 
		rayCallback.m_collisionFilterMask = filter; // define what it collides with
		world->rayTest(start, end, rayCallback);
		for (int i = 0; i < rayCallback.m_hitFractions.size(); i++) {// for each ray intersection
			GameObjectData* gameObjectData = (GameObjectData*)rayCallback.m_collisionObjects[i]->getUserPointer();
			if (gameObjectData) { // if the object is recognisable as a GameObject
								  // add the Intersection data to the array
				std::weak_ptr<GameObject> g = std::dynamic_pointer_cast<GameObject>(gameObjectData->gameObject.lock());
				std::shared_ptr<GameObject> gameObject = g.lock();
				toReturn.push_back(RayCastHit(true, TypeConverter::ConvertToGLMVector3(rayCallback.m_hitNormalWorld[i]), TypeConverter::ConvertToGLMVector3(rayCallback.m_hitPointWorld[i]), (to - from).length() * rayCallback.m_hitFractions[i], gameObject));
			}
		}
		return toReturn;
	}

	const bool PhysicsSystem::GetDrawDebug() const {
		return drawDebug;
	}

	bool PhysicsSystem::RemoveBody(btRigidBody * body) {
		if (world && body) {
			world->removeRigidBody(body);
			return true;
		}
		return false;
	}

	const glm::vec3 PhysicsSystem::GetGravity() const {
		return gravity;
	}

	void PhysicsSystem::SetGravity(const glm::vec3 & grav) {
		if (world) {
			gravity = grav;
			world->setGravity(TypeConverter::ConvertToBulletVector3(gravity));
		}
	}

	void PhysicsSystem::AttachRigidBody(const std::shared_ptr<RigidBody>& rb, const bool & dynamic) {
		if (rb->body) {
			std::cout << "Body already attached" << std::endl;
		}
		else {
			btEmptyShape * emptyShape = new btEmptyShape();
			btCompoundShape * shape = new btCompoundShape();
			btTransform t;
			t.setIdentity();
			btVector3 inertia(0, 0, 0);
			shape->addChildShape(t, emptyShape);

			btScalar bMass = dynamic ? 1.0f : 0.0f;

			btRigidBody* body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(bMass, std::static_pointer_cast<btMotionState>(rb->GetComponent<Transform>().lock()).get(), shape, inertia));
			world->addRigidBody(body);
			rb->body = body;
			shape->setUserPointer(rb->GetData());
			rb->body->setUserPointer(rb->GetData());
			if (!dynamic) rb->body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // if object has no mass make object kinematic
		}
	}

	bool PhysicsSystem::AddBody(btRigidBody * body) {
		if (world && body) {
			world->addRigidBody(body);
			return true;
		}
		return false;
	}

	const glm::vec3 PhysicsSystem::GetGravity() {
		return gravity;
	}

	bool PhysicsSystem::BeginContactCallBack(btManifoldPoint & cp, const btCollisionObject * body1, const btCollisionObject * body2) {
		GameObjectData* gameObjectData1 = (GameObjectData*)body1->getUserPointer(), *gameObjectData2 = (GameObjectData*)body2->getUserPointer();
		if (gameObjectData1 && gameObjectData2) { // if both objects are recognisable as a GameObjects
												  // unwrap the GameObjectData
			std::shared_ptr<GameObject> gameObject1 = gameObjectData1->gameObject.lock();
			std::shared_ptr<GameObject> gameObject2 = gameObjectData2->gameObject.lock();
			// cache the normal and contact point
			glm::vec3 normal = TypeConverter::ConvertToGLMVector3(cp.m_normalWorldOnB);
			glm::vec3 contactPoint = TypeConverter::ConvertToGLMVector3(cp.m_positionWorldOnA);
			Collision collision;
			collision.point = contactPoint;
			// send the collision notifications to the GameObjects
			if (gameObject1) {
				collision.normal = normal;
				collision.distance = cp.m_distance1;
				collision.gameObject = gameObject2;
				gameObject1->OnCollisionEnter(collision);
			}
			if (gameObject2) {
				collision.gameObject = gameObject1;
				collision.distance = cp.getDistance();
				collision.normal = -normal;
				gameObject2->OnCollisionEnter(collision);
			}
			if (!cp.m_userPersistentData) { // create Cache to be used when the Collision ends
				GameObjectsCache *cache = new GameObjectsCache();
				cache->gameObject1 = gameObject1;
				cache->gameObject2 = gameObject2;
				cp.m_userPersistentData = cache;
			}
		}
		return true;
	}

	bool PhysicsSystem::EndContactCallBack(GameObjectsCache * data) {
		if (data) { //if Collision cache was generated
					// unwrap the GameObjectsCache
			std::shared_ptr<GameObject> gameObject1 = data->gameObject1.lock(), gameObject2 = data->gameObject2.lock();
			// create a default collision, since the objects are not colliding anymore the only relevant value if gameObject
			Collision collision;
			// send the collision end notifications to the GameObjects
			if (gameObject1) {
				collision.gameObject = gameObject2;
				gameObject1->OnCollisionExit(collision);
			}
			if (gameObject2) {
				collision.gameObject = gameObject1;
				gameObject2->OnCollisionExit(collision);
			}
			delete data;
			data = nullptr;
		}
		return false;
	}

	void PhysicsSystem::SetDrawDebug(const bool & shouldDraw) {
		drawDebug = shouldDraw;
	}
}