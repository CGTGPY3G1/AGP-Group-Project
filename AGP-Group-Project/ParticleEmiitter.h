#pragma once
#ifndef B00289996B00227422_PARTICLES_H
#define B00289996B00227422_PARTICLES_H

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

namespace B00289996B00227422 {
	class ShaderProgram;
	struct Texture;
	/// <summary> Paricle Vertex Specification</summary>
	struct ParticleVert {
		/// <summary>Initializes a new instance of the <see cref="ParticleVert"/> struct.</summary>
		/// <param name="p">The position.</param>
		/// <param name="tc">The texture coordinate.</param>
		ParticleVert(glm::vec3 p, glm::vec2 tc) : position(p), textureCoord(tc) {}
		glm::vec2 textureCoord;
		glm::vec3 position;
		glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	class Particle {
		friend class ParticleEmitter;
		friend struct IsDead;
	public:
		/// <summary>Initializes a new instance of the <see cref="Particle"/> class.</summary>
		Particle();
		/// <summary>Finalizes an instance of the <see cref="Particle"/> class.</summary>
		~Particle();
		/// <summary>Sets the linear velocity.</summary>
		/// <param name="newLinearVelocity">The new linear velocity.</param>
		void SetLinearVelocity(const glm::vec3 & newLinearVelocity);
		/// <summary>Gets the linear velocity.</summary>
		/// <returns>The linear velocity.</returns>
		const glm::vec3 GetLinearVelocity() const;
		/// <summary>Sets the angular velocity.</summary>
		/// <param name="angle">The angle to rotate by.</param>
		/// <param name="axis">The axis to rotate around.</param>
		void SetAngularVelocity(const float & angle, const glm::vec3 & axis);
		/// <summary>Gets the angle the particle rotates by.</summary>
		/// <returns>the angle</returns>
		const float GetAngle() const;
		/// <summary>Gets the axis the particle rotates around.</summary>
		/// <returns>the axis</returns>
		const glm::vec3 GetAxis() const;
		/// <summary>Sets the size of the particle.</summary>
		/// <param name="size">The size.</param>
		void SetSize(const float & size);
		/// <summary>Sets the start and end sizes of the particle (size will be interpolated from start to end).</summary>
		/// <param name="start">The start size.</param>
		///  <param name="end">The end size.</param>
		void SetSize(const float & start, const float & end);
		/// <summary>Gets the start size.</summary>
		/// <returns>the start size.</returns>
		const float GetStartSize() const;
		/// <summary>Gets the end size.</summary>
		/// <returns>the end size.</returns>
		const float GetEndSize() const;
		/// <summary>Sets the force that will be applied to this particle during its lifetime.</summary>
		/// <param name="force">The force.</param>
		void SetForce(const glm::vec3 & force);
		/// <summary>Sets the start and end forces that will be applied to this particle during its lifetime (forces will be interpolated from start to end).</summary>
		/// <param name="start">The force at particles creation.</param>
		///  <param name="end">The force at particles death.</param>
		void SetForce(const glm::vec3 & start, const glm::vec3 & end);
		/// <summary>Sets the start colour of the particle.</summary>
		/// <param name="newColour">The colour.</param>
		void SetColour(const glm::vec4 & newColour);
		/// <summary>Sets the start and end colours of the particle (colour will be interpolated from start to end).</summary>
		/// <param name="start">The start colour.</param>
		///  <param name="end">The end colour.</param>
		void SetColour(const glm::vec4 & start, const glm::vec4 & end);
		/// <summary>Gets the start colour.</summary>
		/// <returns>the start colour</returns>
		const glm::vec4 GetStartColour() const;
		/// <summary>Gets the end colour.</summary>
		/// <returns>the end colour</returns>
		const glm::vec4 GetEndColour() const;
		/// <summary>Sets the particles life time.</summary>
		/// <param name="newLifeTime">The new life time.</param>
		void SetLifeTime(const float & newLifeTime);
		/// <summary> Gets the particles life time.</summary>
		/// <returns>the particles life time</returns>
		const float GetLifeTime() const;
		/// <summary>Determines whether this particle is alive.</summary>
		/// <returns>true id life time not reached, else false</returns>
		const bool IsAlive() const;
	private:
		/// <summary>Updates the particle</summary>
		/// <param name="deltaTime">The frame time.</param>
		void Update(const float & deltaTime);
		/// <summary>Renders the particle.</summary>
		/// <param name="transform">The emitters transform.</param>
		/// <param name="camPosition">The camera position.</param>
		/// <param name="camUp">The cameras up direction.</param>
		/// <param name="shader">The shader.</param>
		void Render(const glm::mat4 & transform, const glm::vec3 & camPosition, const glm::vec3 & camUp, const std::shared_ptr<ShaderProgram> & shader);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), scale = glm::vec3(1.0f, 1.0f, 1.0f), linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f), angularAxis, moveVector = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec4 startColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), endColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 startForce = glm::vec3(0.0f, 0.0f, 0.0f), endforce = glm::vec3(0.0f, 0.0f, 0.0f);
		float lifeTime = 0.0f, timeToLive = 1.0f, angularVelocity, startSize, endSize, emissionAngle = 0.0f;
		bool alive = true;
	};

	class ParticleEmitter : public ScriptableComponent {
	public:
		/// <summary>Initializes a new instance of the <see cref="ParticleEmitter"/> class.</summary>
		ParticleEmitter();
		/// <summary>Initializes a new instance of the <see cref="ParticleEmitter"/> class.</summary>
		/// <param name="gameObject">The game object this component is attached to.</param>
		ParticleEmitter(std::weak_ptr<GameObject> gameObject);
		/// <summary>Finalizes an instance of the <see cref="ParticleEmitter"/> class.</summary>
		~ParticleEmitter();
		/// <summary>Gets the linear velocity of particles created by this emitter.</summary>
		/// <returns>The linear velocity.</returns>
		const glm::vec3 GetLinearVelocity() const;
		/// <summary>Sets the linear velocity of particles created by this emitter.</summary>
		/// <param name="newLinearVelocity">The new linear velocity.</param>
		void SetLinearVelocity(const glm::vec3 & newLinearVelocity);
		/// <summary>Sets the angular velocity of particles created by this emitter.</summary>
		/// <param name="angle">The angle to rotate by.</param>
		/// <param name="axis">The axis to rotate around.</param>
		void SetAngularVelocity(const float & angle, const glm::vec3 & axis);
		/// <summary>Sets the rate at which  particles are emitted by this emitter.</summary>
		/// <param name="rate">The new emission rate.</param>
		void SetEmissionRate(const float & rate);
		/// <summary>Gets the rate at which  particles are emitted by this emitter.</summary>
		/// <returns>the emission rate</returns>
		const float GetEmissionRate() const;
		/// <summary>Gets the total number of active particles.</summary>
		/// <returns>the number of active particles.</returns>
		const size_t GetNumberOfParticles() const;
		/// <summary>Sets the size of the particle.</summary>
		/// <param name="size">The size.</param>
		void SetSize(const float & size);
		/// <summary>Sets the start and end sizes of particles created by this emitter. (size will be interpolated from start to end).</summary>
		/// <param name="start">The start size.</param>
		///  <param name="end">The end size.</param>
		void SetSize(const float & start, const float & end);
		/// <summary>Gets the start size of particles created by this emitter.</summary>
		/// <returns>the start size.</returns>
		const float GetStartSize() const;
		/// <summary>Gets the end size.</summary>
		/// <returns>the end size.</returns>
		const float GetEndSize() const;
		/// <summary>Sets the emission radius.</summary>
		/// <param name="radius">The radius.</param>
		void SetRadius(const float & radius);
		/// <summary>Sets the force that will be applied to the particles created by this emitter.</summary>
		/// <param name="force">The force.</param>
		void SetForce(const glm::vec3 & force);
		/// <summary>Sets the start and end forces that will be applied to the particles created by this emitter during their lifetime (forces will be interpolated from start to end).</summary>
		/// <param name="start">The force at particles creation.</param>
		///  <param name="end">The force at particles death.</param>
		void SetForce(const glm::vec3 & start, const glm::vec3 & end);
		/// <summary>Sets the texture that will be applied to the particles created by this emitter.</summary>
		/// <param name="newTexture">The new texture.</param>
		void SetTexture(const std::shared_ptr<Texture> & newTexture);
		/// <summary>Sets the colour that will be applied to the particles created by this emitter.</summary>
		/// <param name="newColour">The new colour.</param>
		void SetColour(const glm::vec4 & newColour);
		/// <summary>Sets the start and end colours that will be applied to the particles created by this emitter during their lifetime (colours will be interpolated from start to end).</summary>
		/// <param name="start">The colours at particles creation.</param>
		///  <param name="end">The colours at particles death.</param>
		void SetColour(const glm::vec4 & start, const glm::vec4 & end);
		/// <summary>Gets the start colour.</summary>
		/// <returns>the start colour</returns>
		const glm::vec4 GetStartColour() const;
		/// <summary>Gets the end colour.</summary>
		/// <returns>the end colour</returns>
		const glm::vec4 GetEndColour() const;
		/// <summary>Sets the emission angle. (angle is offset from y acis)</summary>
		/// <param name="angle">The angle.</param>
		void SetEmissionAngle(const float & angle);
		/// <summary>Sets life time of particles created by this emitter.</summary>
		/// <param name="newLifeTime">The new life time.</param>
		void SetParticleLifeTime(const float & newLifeTime);
		/// <summary>Gets the life time of particles created by this emitter.</summary>
		/// <returns>the life time</returns>
		const float GetParticleLifeTime() const;
		/// <summary>Sets the amount of time to wait before emitting particles.</summary>
		/// <param name="delay">The amount of time to wait.</param>
		void SetStartDelay(const float & delay);
		/// <summary>Updates the particle emitter</summary>
		/// <param name="deltaTime">The frame time.</param>
		void Update(const float & deltaTime) override;
		/// <summary>Renders this emitter.</summary>
		void Render();
		const ComponentType Type() const override { return COMPONENT_PARTICLE_EMITTER; }
		const std::string GetName() const override { return "ParticleEmitter"; }
		/// <summary>Resets this emitter.</summary>
		void Reset();
	private:
		/// <summary>Generates the VAO used by this emitter.</summary>
		void Generate();
		glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f), angularAxis, particleScale = glm::vec3(1.0);
		float emissionRate = 0.0f, accumulatedTime = 0.0f, startDelay = 0.0f;
		bool started = false;
		std::list<Particle> particles; // active particles
		std::shared_ptr<ShaderProgram> shader; // current shader
		std::shared_ptr<Texture> texture; // current texture
		float particleLifeLength = 1.0f, angularVelocity, startSize, endSize, emissionAngle = 0.0f, emissionRadius = 0.1f;
		std::vector<ParticleVert> vertices; // vertex specifications
		std::vector<unsigned int> indices;
		static GLuint VAO, VBO, IBO; // vertex array, vertex buffer and index buffer
		glm::vec4 startColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), endColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 startForce = glm::vec3(0.0f, 0.0f, 0.0f), endforce = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	
}
#endif // !B00289996B00227422_PARTICLES_H
