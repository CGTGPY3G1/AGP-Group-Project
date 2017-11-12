#pragma once
#ifndef B00289996_PARTICLES_H
#define B00289996_PARTICLES_H

#include "ScriptableComponent.h"
#include <glm/glm.hpp>


#include <string>
#include <list>
#include <glm\common.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\mat4x4.hpp>
#include <GL\glew.h>
#include <memory>

namespace B00289996 {
	class ShaderProgram;
	struct Texture;
	struct ParticleVert {
		ParticleVert(glm::vec3 p, glm::vec2 tc) : position(p), textureCoord(tc) {}
		glm::vec2 textureCoord;
		glm::vec3 position;
		glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	class Particle {
		friend class ParticleEmitter;
		friend struct IsDead;
	public:
		Particle();
		~Particle();
		void SetLinearVelocity(const glm::vec3 & newLinearVelocity);
		const glm::vec3 GetLinearVelocity() const;
		void SetAngularVelocity(const float & angle, const glm::vec3 & axis);
		const float GetAngularVelocity() const;
		const glm::vec3 GetAngularAxis() const;
		void SetSize(const float & size);
		void SetSize(const float & start, const float & end);
		const float GetStartSize() const;
		const float GetEndSize() const;
		void SetForce(const glm::vec3 & force);
		void SetForce(const glm::vec3 & start, const glm::vec3 & end);
		void SetColour(const glm::vec4 & newColour);
		void SetColour(const glm::vec4 & start, const glm::vec4 & end);
		const glm::vec4 GetStartColour() const;
		const glm::vec4 GetEndColour() const;
		void SetLifeTime(const float & newLifeTime);
		const float GetLifeTime() const;
		const bool IsAlive() const;
	private:
		void Update(const float & deltaTime);
		void Render(const glm::mat4 & transform, const glm::vec3 & camPosition, const glm::vec3 & camUp, const std::shared_ptr<ShaderProgram> & shader);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), scale = glm::vec3(1.0f, 1.0f, 1.0f), linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f), angularAxis, moveVector = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec4 startColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), endColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 startForce = glm::vec3(0.0f, 0.0f, 0.0f), endforce = glm::vec3(0.0f, 0.0f, 0.0f);
		float lifeTime = 0.0f, timeToLive = 1.0f, angularVelocity, startSize, endSize, emissionAngle = 0.0f;
		bool alive = true;
	};

	class ParticleEmitter : public ScriptableComponent {
	public:
		ParticleEmitter();
		ParticleEmitter(std::weak_ptr<GameObject> gameObject);
		~ParticleEmitter();
		void SetLinearVelocity(const glm::vec3 & newLinearVelocity);
		const glm::vec3 GetLinearVelocity() const;
		void SetAngularVelocity(const float & angle, const glm::vec3 & axis);
		void SetEmissionRate(const float & rate);
		const float GetEmissionRate() const;
		const size_t GetNumberOfParticles() const;
		/*void SetEmitterLifeTime(const float & newLifeTime);
		const float GetEmitterLifeTime() const;*/
		void SetSize(const float & size);
		void SetSize(const float & start, const float & end);
		const float GetStartSize() const;
		const float GetEndSize() const;
		void SetRadius(const float & radius);
		void SetForce(const glm::vec3 & force);
		void SetForce(const glm::vec3 & start, const glm::vec3 & end);
		void SetTexture(const std::shared_ptr<Texture> & newTexture);
		void SetColour(const glm::vec4 & newColour);
		void SetColour(const glm::vec4 & start, const glm::vec4 & end);
		const glm::vec4 GetStartColour() const;
		const glm::vec4 GetEndColour() const;
		void SetEmissionAngle(const float & angle);
		void SetParticleLifeTime(const float & newLifeTime);
		const float GetParticleLifeTime() const;
		void SetStartDelay(const float & delay);
		void Update(const float & deltaTime) override;
		void Render();
		const ComponentType Type() const override { return COMPONENT_PARTICLE_EMITTER; }
		const std::string GetName() const override { return "ParticleEmitter"; }
		void Reset();
	private:
		void Generate();
		glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f), angularAxis, particleScale = glm::vec3(1.0);
		float emissionRate = 0.0f, accumulatedTime = 0.0f, startDelay = 0.0f;
		bool started = false;
		std::list<Particle> particles;
		std::shared_ptr<ShaderProgram> shader;
		std::shared_ptr<Texture> texture;
		float particleLifeLength = 1.0f, angularVelocity, startSize, endSize, emissionAngle = 0.0f, emissionRadius = 0.1f;
		std::vector<ParticleVert> vertices;
		std::vector<unsigned int> indices;
		GLuint VAO, VBO, IBO, MBO, CBO;
		glm::vec4 startColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), endColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 startForce = glm::vec3(0.0f, 0.0f, 0.0f), endforce = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	
}
#endif // !B00289996_PARTICLES_H
