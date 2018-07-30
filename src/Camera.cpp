#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
namespace B00289996B00227422 {
	/// <summary>
	/// The main camera{CC2D43FA-BBC4-448A-9D0B-7B57ADF2655C}
	/// </summary>
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
		// convert the FOV to radians
		const float rFOV = glm::radians(fov); 
		// use the same values to set up the camera and frustum viewprojections
		viewprojection = glm::perspective(rFOV, aspect, near, far);
		frustum.DefinePerspective(rFOV, aspect, near, far);
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
		// calculate a glsl compatable view matrix using the cameras objects transform component
		return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
	}

	glm::mat4 Camera::GetProjection() {
		return viewprojection;
	}

	glm::mat4 Camera::GetViewProjection() {
		return viewprojection * GetView();
	}

	Frustum Camera::GetFrustum() {
		// update the frustums location and rotation using the cameras objects transform component
		std::shared_ptr<Transform> transform = GetComponent<Transform>().lock();
		frustum.DefineView(transform->GetPosition(), transform->GetForward(), transform->GetUp());
		return frustum;
	}
}
