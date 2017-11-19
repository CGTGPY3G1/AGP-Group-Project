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
		RenderingSystem();
		~RenderingSystem() {}
		void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) override;
		const unsigned int GetComponentMask() const override;
		void SetShader(std::shared_ptr<ShaderProgram> shader);
		void UpdateTimers(const float deltaTime);
		void UpdateLightingUniforms();
		void UpdateCameraUniforms();
		void ClearData();
		void ResizeBuffers();
		void RenderScene();
		const bool GetWave() const;
		void SetWave(const bool & wave);
		const bool GetBlur() const;
		void SetBlur(const bool & blur);
		const bool GetBloom() const;
		void SetBloom(const bool & bloom);
		const bool GetSwirl() const;
		void SetSwirl(const bool & swirl);
		void SwapFrameBuffers(std::shared_ptr<FrameBuffer> & l, std::shared_ptr<FrameBuffer> & r);
		int GetFrustumCollisionType();
		void SetFrustumCollisionType(const int & type);

	private:
		std::shared_ptr<ShaderProgram> shader, depthShader, cubicDepthShader, brightShader, gaussianBlur, staticWave, blendShader, swirlShader;
		std::vector<std::shared_ptr<PointLight>> pointLights;
		std::vector<std::shared_ptr<SpotLight>> spotLights; 
		std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;
		std::shared_ptr<DirectionLight> directionLight;
		std::shared_ptr<GameObject> cameraObject;
		std::shared_ptr<GameObject> directionLightObject;
		std::vector<std::shared_ptr<GameObject>> allRenderables;
		std::vector<std::shared_ptr<GameObject>> renderables;
		std::shared_ptr<DepthBuffer> depthBuffer;
		std::vector<std::shared_ptr<CubicDepthBuffer>> cubicDepthBuffers;
		std::shared_ptr<FrameBuffer> frameBuffer;
		std::shared_ptr<FrameBuffer> tempBuffers[2];
		const static int MAX_LIGHTS = 5;
		bool wave, bloom, swirl, blur;
		float waveTimer, swirlTimer;
		int collisionType;
	};
}
#endif // !B00289996_RENDERING_SYSTEM_H
