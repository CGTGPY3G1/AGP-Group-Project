#include "Transform.h"
#include <algorithm>
#include <glm\gtx\matrix_decompose.hpp>
#include <glm\ext.hpp>
#include "TypeConverter.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "Bounds.h"
namespace B00289996B00227422 {
	const glm::mat4 Transform::IDENTITY = glm::mat4(1.0f);

	Transform::Transform(const std::weak_ptr<GameObject> & g, const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) : Component(g), btMotionState(), dirty(true) {
		static unsigned int idGen = 0;
		id = ++idGen;
		localPosition = position; localRotation = rotation, localScale = scale;
		SetDirty();
	}

	Transform::~Transform() {
	}

	const glm::vec3 Transform::GetPosition() {
		UpdateTransform();
		return worldPosition;
	}

	const glm::vec3 Transform::GetLocalPosition() const {
		return localPosition;
	}

	void Transform::SetPosition(const glm::vec3 newPosition) {
		SetPosition(newPosition.x, newPosition.y, newPosition.z);
	}

	void Transform::SetPosition(const float & x, const float & y, const float & z) {
		if(parent.use_count() > 0) {
			const glm::vec3 pPos = parent.lock()->GetPosition();
			localPosition.x = x - pPos.x; localPosition.y = y - pPos.y; localPosition.z = z - pPos.z;
		}
		else {
			localPosition.x = x; localPosition.y = y; localPosition.z = z;
		}
		SetDirty();
	}

	void Transform::SetLocalPosition(const glm::vec3 newPosition) {
		SetLocalPosition(newPosition.x, newPosition.y, newPosition.z);
	}

	void Transform::SetLocalPosition(const float & x, const float & y, const float & z) {
		localPosition.x = x, localPosition.y = y, localPosition.z = z;
		SetDirty();
	}

	void Transform::SetRotation(const glm::quat & newRotation) {
		if(parent.use_count() > 0) localRotation = parent.lock()->GetRotation() * glm::inverse(newRotation);
		else localRotation = newRotation;
		SetDirty();
	}

	void Transform::SetLocalRotation(const glm::quat & newRotation) {
		localRotation = newRotation;
		SetDirty();
	}

	void Transform::Translate(const float & x, const float & y, const float & z, const bool & worldSpace) {
		UpdateTransform();
		if(worldSpace) {
			localPosition.x += x;
			localPosition.y += y;
			localPosition.z += z;
		}
		else localPosition += glm::vec3(-glm::rotate(localRotation, (glm::vec4(x, y, z, 0.0f))));
		SetDirty();
	}

	void Transform::Translate(const glm::vec3 & displacement, const bool & worldSpace) {
		Translate(displacement.x, displacement.y, displacement.z, worldSpace);
	}

	void Transform::Rotate(const glm::vec3 & axis, const float & angle) {
		localRotation = glm::rotate(localRotation, glm::radians(angle), axis);
		SetDirty();
	}

	const glm::quat Transform::GetRotation() {
		return worldRotation;
	}

	const glm::quat Transform::GetLocalRotation() const {
		return localRotation;
	}

	void Transform::Scale(const float & scale) {
		this->localScale *= scale;
		SetDirty();
	}

	void Transform::SetScale(const float & x, const float & y, const float & z) {
		if(parent.use_count() > 0) {
			const glm::vec3 pScale = parent.lock()->GetScale();
			localScale.x = x / pScale.x; localScale.y = y / pScale.y; localScale.z = z / pScale.z;
		}
		else {
			localScale.x = x; localScale.y = y; localScale.z = z;
		}
		SetDirty();
	}

	void Transform::SetScale(const glm::vec3 & scale) {
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetLocalScale(const float & x, const float & y, const float & z) {
		localScale.x = x; localScale.y = y; localScale.z = z;
		SetDirty();
	}

	void Transform::SetLocalScale(const glm::vec3 & scale) {
		localScale = scale;
		SetDirty();
	}

	const glm::vec3 Transform::GetScale() {
		UpdateTransform();
		return worldScale;
	}

	const glm::vec3 Transform::GetLocalScale() const {
		return localScale;
	}

	const glm::vec3 Transform::GetForward() {
		UpdateTransform();
		return forward;
	}

	void Transform::SetForward(const glm::vec3 & newForward, const glm::vec3 & worldUp) {
		glm::mat4 toDecompose = glm::lookAt(GetPosition(), GetPosition() - newForward, worldUp);
		glm::decompose(toDecompose, glm::vec3(), localRotation, glm::vec3(), glm::vec3(), glm::vec4());
		SetDirty();
	}

	const glm::vec3 Transform::GetUp() {
		UpdateTransform();
		return up;
	}

	const glm::vec3 Transform::GetRight() {
		UpdateTransform();
		return right;
	}

	const glm::vec3 Transform::TransformToWorldPoint(const glm::vec3 & point) {
		UpdateTransform();
		return glm::vec3(worldTransform * glm::vec4(point, 0.0f));
	}

	const glm::vec3 Transform::TransformToLocalPoint(const glm::vec3 & point) {
		UpdateTransform();
		return glm::vec3(world2Local * glm::vec4(point, 0.0f));
	}

	void Transform::SetParent(const std::weak_ptr<Transform> & newParent) {
		if(parent.use_count() > 0) parent.lock()->RemoveChild(std::static_pointer_cast<Transform>(shared_from_this()));
		parent = newParent;
		bool added = false;
		if(!parent.expired()) {
			std::shared_ptr<Transform> p = parent.lock();
			if(p) {
				p->AddChild(std::static_pointer_cast<Transform>(shared_from_this()));
				localPosition -= p->GetPosition();
				added = true;
			}
		}
		SetDirty();
	}

	const std::weak_ptr<Transform> Transform::GetParent() const {
		return parent;
	}

	void Transform::SlerpRotation(const glm::quat & slerpTo, const float & alpha) {
		localRotation = glm::slerp(localRotation, slerpTo, alpha);
		SetDirty();
	}

	glm::quat Transform::LookAt(const glm::vec3 & eye, const glm::vec3 & centre, const glm::vec3 & up) {
		glm::quat toReturn;
		glm::mat4 toDecompose = glm::lookAt(eye, centre, up);
		glm::decompose(toDecompose, glm::vec3(), toReturn, glm::vec3(), glm::vec3(), glm::vec4());
		return toReturn;
	}

	glm::quat Transform::SlerpQuaternion(const glm::quat & slerpFrom, const glm::quat & slerpTo, const float & alpha) {
		return glm::slerp(slerpFrom, slerpTo, alpha);;
	}

	void Transform::LerpPosition(const glm::vec3 & lerpTo, const float & alpha) {
		localPosition = glm::lerp(localPosition, lerpTo, alpha);
		SetDirty();
	}

	glm::vec3 Transform::SlerpVec3(const glm::vec3 & slerpFrom, const glm::vec3 & slerpTo, const float & alpha) {
		return  glm::slerp(slerpFrom, slerpTo, alpha);
	}

	void Transform::setWorldTransform(const btTransform & worldTrans) {
		UpdateTransform();
		float data[16];
		worldTrans.getOpenGLMatrix(data);
		glm::mat4 m = glm::make_mat4(data);
		if(parent.use_count() > 0) {
			glm::mat4 m2 = glm::inverse(parent.lock()->GetWorldTransform());
			glm::decompose(m2 * m, glm::vec3(), localRotation, localPosition, glm::vec3(), glm::vec4());
		}
		else {
			SetPosition(TypeConverter::ConvertToGLMVector3(worldTrans.getOrigin()));
			SetRotation(TypeConverter::ConvertToGLMQuat(worldTrans.getRotation()));
		}
		SetDirty();
	}

	OBB Transform::TransformOBB(const OBB & obb) {
		OBB toReturn;
		toReturn.centre += GetPosition();
		const glm::vec3 scale = GetScale();
		toReturn.halfExtents = glm::vec3(obb.halfExtents.x * std::abs(scale.x), obb.halfExtents.y * std::abs(scale.y), obb.halfExtents.z * std::abs(scale.z));
		toReturn.rotation = (obb.rotation * GetRotation());
		return toReturn;
	}

	Sphere Transform::TransformSphere(const Sphere & sphere) {
		Sphere toReturn; // create a new Sphere to return
						 // translate and scale the Sphere
		toReturn.centre = sphere.centre + GetPosition();
		// scale the radius by the largest of the scales components 
		glm::vec3 scale = GetScale();
		toReturn.radius = sphere.radius * std::max(std::abs(scale.x), std::max(std::abs(scale.y), std::abs(scale.z)));
		return toReturn; // return the new Sphere
	}

	void Transform::AddChild(std::weak_ptr<Transform> child) {
		if(std::find_if(children.begin(), children.end(), [&](const std::weak_ptr<Transform>& other) { return !other.expired() && other.lock()->id == child.lock()->id; }) == children.end()) {
			children.push_back(child);
		};
		SetDirty();
	}

	void Transform::RemoveChild(std::weak_ptr<Transform> child) {
		std::vector<std::weak_ptr<Transform>>::iterator it = std::remove_if(children.begin(), children.end(), [&](const std::weak_ptr<Transform>& other) { return other.expired() && other.lock()->id == child.lock()->id; });
		if(it != children.end()) {
			(*it).lock()->SetParent(std::shared_ptr<Transform>());
			children.erase(it);
		};
	}

	void Transform::RemoveChild(const unsigned int index) {
		if(index < children.size()) children.erase(children.begin() + index);
	}

	const size_t Transform::NumberOfChildren() const {
		return children.size();
	}

	const std::weak_ptr<Transform> Transform::GetChild(const unsigned int index) const {
		if(index < children.size()) return children[index];
		return std::weak_ptr<Transform>();
	}

	const std::vector<std::weak_ptr<Transform>> Transform::GetChildren() const {
		return children;
	}


	const glm::mat4 & Transform::GetWorldTransform() {
		if (dirty) UpdateTransform();
		return worldTransform;
	}

	const glm::mat3 & Transform::GetNormalMatrix() {
		if(dirty) UpdateTransform();
		return normalmatrix;
	}

	void Transform::getWorldTransform(btTransform & worldTrans) {
		UpdateTransform();
		worldTrans.setOrigin(TypeConverter::ConvertToBulletVector3(worldPosition));
		worldTrans.setRotation(TypeConverter::ConvertToBulletQuat(worldRotation));
	}

	void Transform::UpdateTransform() {
		if(dirty) {
			localRotation = glm::normalize(localRotation);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), localPosition);
			glm::mat4 rotationMatrix = glm::mat4_cast(localRotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), localScale);
			localTransform = translationMatrix * rotationMatrix * scaleMatrix;
			if(parent.use_count() > 0) {
				std::shared_ptr<Transform> p = parent.lock();
				if(p) {
					p->UpdateTransform();
					worldTransform = p->worldTransform * localTransform;
					worldRotation = glm::normalize(p->worldRotation * localRotation);
					worldScale.x = p->worldScale.x * localScale.x;
					worldScale.y = p->worldScale.y * localScale.y;
					worldScale.z = p->worldScale.z * localScale.z;
					
				}
				else {
					worldTransform = localTransform;
					worldRotation = glm::normalize(localRotation);
					worldScale = localScale;
				}
			}
			else {
				worldTransform = localTransform;
				worldRotation = glm::normalize(localRotation);
				worldScale = localScale;
			}		
			normalmatrix = glm::mat3_cast(worldRotation);
			world2Local = glm::inverse(worldTransform);
			worldPosition = worldTransform[3];
			forward = glm::normalize(normalmatrix * glm::vec3(0.0f, 0.0f, 1.0f)), up = glm::normalize(normalmatrix * glm::vec3(0.0f, 1.0f, 0.0f)), right = glm::normalize(normalmatrix * glm::vec3(1.0f, 0.0f, 0.0f));
			dirty = false;
		}
	}

	void Transform::SetDirty() {
		if(!dirty) {
			dirty = true;
			for(size_t i = 0; i < children.size(); i++) {
				std::shared_ptr<Transform> c = children[i].lock();
				if(c) c->SetDirty();
			}
		}
	}
}
