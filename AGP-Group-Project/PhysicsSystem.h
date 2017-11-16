#pragma once
#ifndef B00289996_PHYSICS_SYSTEM_H
#define B00289996_PHYSICS_SYSTEM_H
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <glm\vec3.hpp>
#include <memory>
#include <vector>
#include "Message.h"

namespace B00289996 {
	class GameObject;
	struct Collision;
	struct RayCastHit;
	class DebugDraw;
	struct GameObjectsCache;
	struct GameObjectData;
	/// <summary>
	/// Facade for Bullet Physics Libraries 
	/// </summary>
	class PhysicsSystem {
		friend class RigidBody;
	public:
		/// <summary>
		/// Gets the Singleton instance.
		/// </summary>
		/// <returns>The Singleton instance</returns>
		static PhysicsSystem & GetInstance();

		/// <summary>
		/// Finalizes an instance of the <see cref="PhysicsSystem"/> class.
		/// </summary>
		~PhysicsSystem();
		/// <summary>
		/// Steps the physics simularion by the specified delta time.
		/// </summary>
		/// <param name="deltaTime">The delta time.</param>
		void Update(const float & deltaTime);
		/// <summary>
		/// Draws the Debug Renderer.
		/// </summary>
		void Draw();
		/// <summary>
		/// Raycasts returning the closest hit object (if any)
		/// </summary>
		/// <param name="from">the start position.</param>
		/// <param name="to">the end position.</param>
		/// <param name="filter">The collision filter.</param>
		/// <returns>the closest hit object (if any)</returns>
		RayCastHit Raycast(const glm::vec3 & from, const glm::vec3 & to, const int & filter = btBroadphaseProxy::AllFilter);
		/// <summary>
		/// Raycasts returning all hit objects (if any)
		/// </summary>
		/// <param name="from">the start position.</param>
		/// <param name="to">the end position.</param>
		/// <param name="filter">The collision filter.</param>
		/// <returns>all hit object (if any)</returns>
		std::vector<RayCastHit> RaycastAll(const glm::vec3 & from, const glm::vec3 & to, const int & filter = btBroadphaseProxy::AllFilter);
		/// <summary>
		/// enable or disable debug drawing
		/// </summary>
		/// <param name="shouldDraw">should it be drawn.</param>
		void SetDrawDebug(const bool & shouldDraw);
		/// <summary>
		/// Gets the draw debuging flag.
		/// </summary>
		/// <returns>true if drawing, else false</returns>
		const bool GetDrawDebug() const;
		/// <summary>
		/// Removes the body from the world.
		/// </summary>
		/// <param name="body">The body.</param>
		/// <returns>true if body was removed, else false</returns>
		bool RemoveBody(btRigidBody * body);

		/// <summary>
		/// Adds the body to the world.
		/// </summary>
		/// <param name="body">The body.</param>
		/// <returns>true if body was added, else false</returns>
		bool AddBody(btRigidBody * body);
		/// <summary>
		/// Gets the gravity.
		/// </summary>
		/// <returns>the gravity</returns>
		const glm::vec3 GetGravity();
		const glm::vec3 GetGravity() const;
		/// <summary>
		/// Sets the gravity.
		/// </summary>
		/// <param name="grav">The gravity.</param>
		void SetGravity(const glm::vec3 & grav);
	private:
		/// <summary>
		/// Adds a sphere to the specified RigidBody.
		/// </summary>
		/// <param name="gameObject">The RigidBody.</param>
		/// <param name="offset">The colliders centre offset.</param>
		/// <param name="radius">The radius.</param>
		/// <param name="mass">The mass.</param>
		void AddSphere(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const float & radius = 1.0f, const float & mass = 1.0f);
		/// <summary>
		/// Adds a box to the specified RigidBody.
		/// </summary>
		/// <param name="gameObject">The RigidBody.</param>
		/// <param name="offset">The colliders centre offset.</param>
		/// <param name="dimensions">The dimensions of the box.</param>
		/// <param name="mass">The mass.</param>
		void AddBox(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass = 1.0f);
		/// <summary>
		/// Adds a capsule to the specified RigidBody.
		/// </summary>
		/// <param name="gameObject">The RigidBody.</param>
		/// <param name="offset">The colliders centre offset.</param>
		/// <param name="height">The height.</param>
		/// <param name="radius">The radius.</param>
		/// <param name="mass">The mass.</param>
		void AddCapsule(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const float & height, const float & radius, const float & mass = 1.0f);
		/// <summary>
		/// Adds the cylinder to the specified RigidBody.
		/// </summary>
		/// <param name="gameObject">The RigidBody.</param>
		/// <param name="offset">The colliders centre offset.</param>
		/// <param name="dimensions">The dimensions of the cylinder.</param>
		/// <param name="mass">The mass.</param>
		void AddCylinder(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass = 1.0f);
		/// <summary>
		/// Adds a character controller to the specified RigidBody.
		/// </summary>
		/// <param name="gameObject">The RigidBody.</param>
		/// <param name="offset">The colliders centre offset.</param>
		/// <param name="height">The height.</param>
		/// <param name="radius">The radius.</param>
		/// <param name="mass">The mass.</param>
		void AddCharacterController(const std::shared_ptr<RigidBody> & rb, const glm::vec3 & offset, const float & height, const float & radius, const float & mass = 1.0f);
		void AttachRigidBody(const std::shared_ptr<RigidBody> & rb, const bool & dynamic);
		/// <summary>
		/// Begin contact call back.
		/// </summary>
		/// <param name="cp">The contact manifold.</param>
		/// <param name="body1">The first colliding body.</param>
		/// <param name="body2">The second colliding body.</param>
		/// <returns>an irrelevant value not used but required by Bullet Physics</returns>
		static bool BeginContactCallBack(btManifoldPoint & cp, const btCollisionObject * body1, const btCollisionObject * body2);
		/// <summary>
		/// End contact call back.
		/// </summary>
		/// <param name="data">data about the objects that were colliding.</param>
		/// <returns>an irrelevant value not used but required by Bullet Physics</returns>
		static bool EndContactCallBack(GameObjectsCache * data);
		/// <summary>
		/// Prevents a default instance of the <see cref="PhysicsSystem"/> class from being created.
		/// </summary>
		PhysicsSystem();
		glm::vec3 gravity = glm::vec3(0.0f, -0.997f, 0.0f);
		// Physics World Configuration and Utily classes
		btDynamicsWorld* world;
		btDispatcher* dispatcher;
		btCollisionConfiguration* collisionConfig;
		btBroadphaseInterface* broadphase;
		btConstraintSolver* solver;
		bool drawDebug = false;
	};
}
#endif // !B00289996_PHYSICS_SYSTEM_H
