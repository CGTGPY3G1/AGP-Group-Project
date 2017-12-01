#pragma once
#ifndef B00289996B00227422_CAMERA_H
#define B00289996B00227422_CAMERA_H
#include "Component.h"
#include "Frustum.h"
#include <glm\mat4x4.hpp>
#include <memory>

namespace B00289996B00227422 {
	class ShaderProgram;
	class Material;

	class Mesh;
	struct Texture;
	class Camera : public Component {
	public:
		/// <summary>Initializes a new instance of the <see cref="Camera"/> class.</summary>
		/// <param name="g">The GameObject this component is attached to.</param>
		Camera(const std::weak_ptr<GameObject> & g);
		/// <summary>Finalizes an instance of the <see cref="Camera"/> class.</summary>
		~Camera();
		const ComponentType Type() const override { return COMPONENT_CAMERA; }
		const std::string GetName() const override { return "Camera"; }
		/// <summary>Sets the cameras viewprojection values.</summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="near">The near plane.</param>
		/// <param name="">The far plane.</param>
		/// <param name="fov">The field of view.</param>
		void SetProjection(const float & width = 1280.0f, const float & height = 720.0f, const float & near = 0.1f, const float & far = 50.0f, const float & fov = 60.0f);
		/// <summary>Sets this camera as the main camera. allowing it to be accessed useing Camera::mainCamera or Camera::GetMainCamera()</summary>
		void SetAsMain();
		/// <summary>Determines whether this is the main camera or not</summary>
		/// <returns>true is this is the main camera, else false</returns>
		bool IsMainCamera();
		/// <summary>Gets the main camera.</summary>
		/// <returns>the main camera if there is one, else an empty weak pointer</returns>
		static std::weak_ptr<Camera> GetMainCamera();
		bool operator == (const Camera & other);
		bool operator == (const std::shared_ptr<Camera> & other);
		/// <summary>Gets the cameras width.</summary>
		/// <returns>the width</returns>
		const float GetWidth() const;
		/// <summary>Gets the cameras height.</summary>
		/// <returns>the cameras height</returns>
		const float GetHeight() const;
		/// <summary>Gets the view matrix of this camera.</summary>
		/// <returns>the view matrix</returns>
		glm::mat4 GetView();
		/// <summary>Gets the viewprojection matrix of this camera.</summary>
		/// <returns>the viewprojection matrix</returns>
		glm::mat4 GetProjection();
		/// <summary>Gets the combined view viewprojection matrix of this camera.</summary>
		/// <returns>the combined view & viewprojection matrix</returns>
		glm::mat4 GetViewProjection();
		/// <summary>Gets the cameras frustum.</summary>
		/// <returns>the frustum</returns>
		Frustum GetFrustum();
		
	private:
		static std::shared_ptr<Camera> mainCamera;
		glm::mat4 viewprojection; //  the cameras viewprojection matrix
		Frustum frustum;
		// viewprojection values
		float width = 1280, height = 720, near = 0.1f, far = 50.0f, fov = 60.0f;
	};
}

#endif // !B00289996B00227422_CAMERA_H