#pragma once
#ifndef B00289996_CAMERA_H
#define B00289996_CAMERA_H
#include "Component.h"
#include "Frustum.h"
#include <glm\mat4x4.hpp>
#include <memory>

namespace B00289996 {
	class ShaderProgram;
	class Material;

	class Mesh;
	struct Texture;
	class Camera : public Component {
	public:
		Camera(const std::weak_ptr<GameObject> & g);
		~Camera();
		const ComponentType Type() const override { return COMPONENT_CAMERA; }
		const std::string GetName() const override { return "Camera"; }
		void SetProjection(const float & width = 1280.0f, const float & height = 720.0f, const float & near = 0.1f, const float & far = 50.0f, const float & fov = 60.0f);
		void SetAsMain();
		bool IsMainCamera();
		static std::weak_ptr<Camera> GetMainCamera();
		bool operator == (const Camera & other);
		bool operator == (const std::shared_ptr<Camera> & other);
		const float GetWidth() const;
		const float GetHeight() const;
		glm::mat4 GetView();
		glm::mat4 GetProjection();
		glm::mat4 GetViewProjection();
		Frustum GetFrustum();
		static std::shared_ptr<Camera> mainCamera;
	private:
		glm::mat4 projection;
		Frustum frustum;
		float width = 1280, height = 720, near = 0.1f, far = 50.0f, fov = 60.0f;
	};
}

#endif // !B00289996_CAMERA_H