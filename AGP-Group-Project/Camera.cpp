#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
namespace B00289996 {
	std::shared_ptr<Camera> Camera::mainCamera;

	Camera::Camera(const std::weak_ptr<GameObject>& g) : Component(g) {
		SetProjection(width, height, near, far, fov);
		//if(!mainCamera) SetAsMain();
	}

	Camera::~Camera() {
	}

	void Camera::SetProjection(const float & width, const float & height, const float & near, const float & far, const float & fov) {
		this->width = width; this->height = height; this->near = near; this->far = far; this->fov = fov;
		const float aspect = width / height;
		projection = glm::perspective(glm::radians(fov), aspect, near, far);
		frustum.DefinePerspective(glm::radians(fov), aspect, near, far);
	}

	void Camera::SetAsMain() {
		Camera::mainCamera = std::static_pointer_cast<Camera>(shared_from_this());
	}

	bool Camera::IsMainCamera() {
		return (mainCamera && std::static_pointer_cast<Camera>(shared_from_this()) == mainCamera) ? true : false;
	}

	std::weak_ptr<Camera> Camera::GetMainCamera() {
		return mainCamera;
	}

	bool Camera::operator==(const Camera & other) {
		return compID == other.compID;
	}

	bool Camera::operator==(const std::shared_ptr<Camera>& other) {
		return compID == other->compID;
	}

	const float Camera::GetWidth() const {
		return width;
	}

	const float Camera::GetHeight() const {
		return height;
	}

	glm::mat4 Camera::GetView() {
		std::shared_ptr<Transform> transform = GetComponent<Transform>().lock();
		return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
	}

	glm::mat4 Camera::GetProjection() {
		return projection;
	}

	glm::mat4 Camera::GetViewProjection() {
		return projection * GetView();
	}

	Frustum Camera::GetFrustum() {
		std::shared_ptr<Transform> transform = GetComponent<Transform>().lock();
		frustum.DefineView(transform->GetPosition(), transform->GetForward(), transform->GetUp());
		return frustum;
	}
}
