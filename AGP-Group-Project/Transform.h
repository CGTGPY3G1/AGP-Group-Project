#pragma once
#ifndef B002899996_TRANSFORM_H
#define B002899996_TRANSFORM_H
#include "Component.h"
#include <memory>
#include <vector>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <btBulletDynamicsCommon.h>
namespace B00289996 {
	struct OBB;
	struct Sphere;
	class Transform : public Component, public btMotionState {
		friend class TypeConverter;
	public:
		Transform(const std::weak_ptr<GameObject> & g, const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		~Transform();
		const glm::vec3 GetPosition();
		const glm::vec3 GetLocalPosition() const;
		void SetPosition(const glm::vec3 newPosition);
		void SetPosition(const float & x, const float & y, const float & z);
		void SetLocalPosition(const glm::vec3 newPosition);
		void SetLocalPosition(const float & x, const float & y, const float & z);
		const glm::quat GetRotation();
		const glm::quat GetLocalRotation() const;
		void SetRotation(const glm::quat & newRotation);
		void SetLocalRotation(const glm::quat & newRotation);
		void Translate(const float & x, const float & y, const float & z, const bool & worldSpace = false);
		void Translate(const glm::vec3 & displacement, const bool & worldSpace = false);
		void Rotate(const glm::vec3 & axis, const float & angle);
		void Scale(const float & scale);
		void SetScale(const float & x, const float & y, const float & z);
		void SetScale(const glm::vec3 & scale);
		void SetLocalScale(const float & x, const float & y, const float & z);
		void SetLocalScale(const glm::vec3 & scale);
		const glm::vec3 GetScale();
		const glm::vec3 GetLocalScale() const;
		const glm::vec3 GetForward();
		void SetForward(const glm::vec3 & newForward, const glm::vec3 & worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::vec3 GetUp();
		const glm::vec3 GetRight();
		const glm::vec3 TransformToWorldPoint(const glm::vec3 & point);
		const glm::vec3 TransformToLocalPoint(const glm::vec3 & point);
		void SetParent(const std::weak_ptr<Transform> & newParent);
		const std::weak_ptr<Transform> GetParent() const;
		void SlerpRotation(const glm::quat & slerpTo, const float & alpha);
		static glm::quat LookAt(const glm::vec3 & eye, const glm::vec3 & centre, const glm::vec3 & up = glm::vec3(0.0f, 1.0f, 0.0f));
		static glm::quat SlerpQuaternion(const glm::quat & slerpFrom, const glm::quat & slerpTo, const float & alpha);
		void LerpPosition(const glm::vec3 & lerpTo, const float & alpha);
		static glm::vec3 SlerpVec3(const glm::vec3 & slerpFrom, const glm::vec3 & slerpTo, const float & alpha);
		const size_t NumberOfChildren() const;
		const std::weak_ptr<Transform> GetChild(const unsigned int index) const;
		const std::vector<std::weak_ptr<Transform>> GetChildren() const;
		const glm::mat4 & GetWorldTransform();
		const glm::mat3 & GetNormalMatrix();
		void getWorldTransform(btTransform & worldTrans) override;
		void UpdateTransform();
		void setWorldTransform(const btTransform & worldTrans) override;
		static const glm::mat4 IDENTITY;
		const ComponentType Type() const override { return COMPONENT_TRANSFORM; }
		const std::string GetName() const override { return "Transform"; }
		OBB TransformOBB(const OBB & obb);
		Sphere TransformSphere(const Sphere & sphere);
	protected:
		void AddChild(std::weak_ptr<Transform> child);
		void RemoveChild(std::weak_ptr<Transform> child);
		void RemoveChild(const unsigned int index);
		void SetDirty();
		glm::vec3 localPosition = glm::vec3(0.0f), worldPosition, localScale = glm::vec3(1.0f), worldScale = glm::vec3(1.0f), forward, up, right;
		glm::quat localRotation, worldRotation;
		glm::mat4 worldTransform, localTransform, world2Local;
		glm::mat3 normalmatrix;
		bool dirty = true;
		unsigned int id;
		std::weak_ptr<Transform> parent;
		std::vector<std::weak_ptr<Transform>> children;
	private:
		glm::vec3 NULL_VEC3;
		glm::vec4 NULL_VEC4;
	};
}

#endif // !B002899996_TRANSFORM_H
