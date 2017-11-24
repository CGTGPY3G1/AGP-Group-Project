#pragma once
#ifndef B00289996_RENDERING_SYSTEM_H
#define B00289996_RENDERING_SYSTEM_H

#include "System.h"

namespace B00289996 {
	struct SpotLight;
	struct Texture;
	struct DirectionLight;
	struct PointLight;
	class ParticleEmitter;
	class ShaderProgram;
	class FrameBuffer;
	class DepthBuffer;
	class CubicDepthBuffer;
	class RenderingSystem : public System {
	public:

		/// <summary> Initializes a new instance of the <see cref="RenderingSystem"/> class. </summary>
		RenderingSystem();
		/// <summary> Finalizes an instance of the <see cref="RenderingSystem"/> class. </summary>
		~RenderingSystem() {}
		/// <summary> Stores all relevant gameobjects and components to be processed further.  performs basic culling operations </summary>
		/// <param name="gameObjects">The game objects.</param>
		void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) override;
		/// <summary> Gets the component mask. </summary>
		/// <returns>the component mask</returns>
		const unsigned int GetComponentMask() const override;
		/// <summary> Sets the main shader. </summary>
		/// <param name="shader">The shader.</param>
		void SetShader(std::shared_ptr<ShaderProgram> shader);
		/// <summary> Updates any shader timers. </summary>
		/// <param name="deltaTime">The delta time.</param>
		void UpdateTimers(const float deltaTime);
		/// <summary> Updates the lighting uniforms. </summary>
		void UpdateLightingUniforms();
		/// <summary> Updates the camera uniforms. </summary>
		void UpdateCameraUniforms();
		/// <summary> Clears all renderable data. </summary>
		void ClearData();
		/// <summary>Resizes the frame buffers. </summary>
		void ResizeBuffers();
		/// <summary> Renders the scene. </summary>
		void RenderScene();
		/// <summary>  Gets the boolean value used to toggle the wave effect. </summary>
		/// <returns>true if wave effect is in use, else false.</returns>
		const bool GetWave() const;
		/// <summary> Sets the boolean value used to toggle the wave effect.</summary>
		/// <param name="wave">true if wave effect is to be used, else false.</param>
		void SetWave(const bool & wave);
		/// <summary> Gets the boolean value used to toggle the blur effect. </summary>
		/// <returns>true if blur effect is in use, else false.</returns>
		const bool GetBlur() const;
		/// <summary> Sets the boolean value used to toggle the blur effect. </summary>
		/// <param name="blur">true if blur effect is to be used, else false.</param>
		void SetBlur(const bool & blur);
		/// <summary> Gets the boolean value used to toggle the bloom effect. </summary>
		/// <returns>true if bloom effect is in use, else false.</returns>
		const bool GetBloom() const;
		/// <summary> Sets the boolean value used to toggle the bloom effect. </summary>
		/// <param name="blur">true if bloom effect is to be used, else false.</param>
		void SetBloom(const bool & bloom);
		/// <summary> Gets the boolean value used to toggle the swirl effect. </summary>
		/// <returns>true if swirl effect is in use, else false.</returns>
		const bool GetSwirl() const;
		/// <summary> Sets the boolean value used to toggle the swirl effect. </summary>
		/// <param name="blur">true if swirl effect is to be used, else false.</param>
		void SetSwirl(const bool & swirl);
		/// <summary> Swaps 2 frame buffers using a temporary variable as storage. </summary>
		/// <param name="l">The first buffer.</param>
		/// <param name="r">The second buffer.</param>
		void SwapFrameBuffers(std::shared_ptr<FrameBuffer> & l, std::shared_ptr<FrameBuffer> & r);
		/// <summary> Gets the boolean value used to toggle the frustum collision type. (Fully Enclosed = true / Intersected = false) </summary>
		/// <returns>a boolean value representing the collision type</returns>
		bool GetFrustumCollisionType();
		/// <summary> Sets the boolean value used to toggle the frustum collision type. (Fully Enclosed = true / Intersected = false)</summary>
		/// <param name="type">a boolean value representing the collision type.</param>
		void SetFrustumCollisionType(const bool & type);
		/// <summary> Gets the boolean value used to toggle the shadows. </summary>
		/// <returns>true if shadows effect is in use, else false.</returns>
		const bool GetCastShadow() const;
		/// <summary> Sets the boolean value used to toggle the shadow effect. </summary>
		/// <param name="castShadow">true if the shadow effect is to be used, else false.</param>
		void SetCastShadow(const bool & castShadow);
	private:
		// temporary renderable representations, refreshed every frame
		std::vector<std::shared_ptr<PointLight>> pointLights;
		std::vector<std::shared_ptr<SpotLight>> spotLights; 
		std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
		std::shared_ptr<DirectionLight> directionLight;
		std::shared_ptr<GameObject> cameraObject;
		std::shared_ptr<GameObject> directionLightObject;
		std::vector<std::shared_ptr<GameObject>> allRenderables;
		std::vector<std::shared_ptr<GameObject>> renderables;
		//shaders
		std::shared_ptr<ShaderProgram> shader, depthShader, cubicDepthShader, brightShader, gaussianBlur, staticWave, blendShader, swirlShader;
		std::shared_ptr<DepthBuffer> depthBuffer;
		std::vector<std::shared_ptr<CubicDepthBuffer>> cubicDepthBuffers;
		std::shared_ptr<FrameBuffer> frameBuffer;
		std::shared_ptr<FrameBuffer> tempBuffers[2];
		const static int MAX_LIGHTS = 5;
		bool wave, bloom, swirl, blur, shadow, collisionType;
		float waveTimer, swirlTimer;
	};
}
#endif // !B00289996_RENDERING_SYSTEM_H
