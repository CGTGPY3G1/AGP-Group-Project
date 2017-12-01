#include "ParticleEmiitter.h"
#include "FileLoader.h"
#include "Shader.h"
#include "Texture.h"
#include <algorithm>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtx\color_space.hpp>
#include "OpenGLUtility.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
namespace B00289996B00227422 {
	GLuint ParticleEmitter::VAO = 0, ParticleEmitter::VBO = 0, ParticleEmitter::IBO = 0;
	ParticleEmitter::ParticleEmitter() {
	}

	ParticleEmitter::ParticleEmitter(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject){
		shader = FileLoader::GetInstance().LoadShader("Shaders//Particles.vert", "Shaders//Particles.frag");
		if(VAO == 0) Generate();
	}

	ParticleEmitter::~ParticleEmitter() {
		glDeleteBuffers(1, &IBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void ParticleEmitter::SetLinearVelocity(const glm::vec3 & newLinearVelocity) {
		linearVelocity = newLinearVelocity;
	}

	const glm::vec3 ParticleEmitter::GetLinearVelocity() const {
		return linearVelocity;
	}

	void ParticleEmitter::SetAngularVelocity(const float & angle, const glm::vec3 & axis) {
		angularVelocity = angle; angularAxis = axis;
	}

	void ParticleEmitter::SetEmissionRate(const float & rate) {
		emissionRate = rate;
	}

	const float ParticleEmitter::GetEmissionRate() const {
		return emissionRate;
	}

	const size_t ParticleEmitter::GetNumberOfParticles() const {
		return particles.size();
	}

	void ParticleEmitter::SetSize(const float & size) {
		startSize = size; endSize = size;
	}

	void ParticleEmitter::SetSize(const float & start, const float & end) {
		startSize = start; endSize = end;
	}

	const float ParticleEmitter::GetStartSize() const {
		return startSize;
	}

	const float ParticleEmitter::GetEndSize() const {
		return endSize;
	}

	void ParticleEmitter::SetRadius(const float & radius) {
		emissionRadius = radius;
	}

	void ParticleEmitter::SetForce(const glm::vec3 & force) {
		startForce = force; endforce = force;
	}

	void ParticleEmitter::SetForce(const glm::vec3 & start, const glm::vec3 & end) {
		startForce = start; endforce = end;
	}

	void ParticleEmitter::SetTexture(const std::shared_ptr<Texture>& newTexture) {
		texture = newTexture;
	}

	void ParticleEmitter::SetColour(const glm::vec4 & newColour) {
		startColour = newColour; endColour = newColour;
	}

	void ParticleEmitter::SetColour(const glm::vec4 & start, const glm::vec4 & end) {
		startColour = start; endColour = end;
	}

	const glm::vec4 ParticleEmitter::GetStartColour() const {
		return startColour;
	}

	const glm::vec4 ParticleEmitter::GetEndColour() const {
		return endColour;
	}

	void ParticleEmitter::SetEmissionAngle(const float & angle) {
		emissionAngle = angle;
	}

	void ParticleEmitter::SetParticleLifeTime(const float & newLifeTime) {
		particleLifeLength = newLifeTime;
	}

	const float ParticleEmitter::GetParticleLifeTime() const {
		return particleLifeLength;
	}

	void ParticleEmitter::SetStartDelay(const float & delay) {
		startDelay = delay;
	}

	void ParticleEmitter::Update(const float & deltaTime) {
		if(started) {
			accumulatedTime += deltaTime;
			if(startDelay > 0.0f) startDelay -= deltaTime;
			while(accumulatedTime >= emissionRate) {
				if(startDelay <= 0.0f) {
					Particle p;
					p.position = glm::sphericalRand(emissionRadius);
					if(p.position.y < 0.0f) p.position.y = -p.position.y;
					p.SetForce(startForce, endforce);
					if(emissionAngle > 0.000001f || emissionAngle < -0.000001f) {
						glm::vec3 axis = glm::cross(glm::normalize(linearVelocity), glm::normalize(p.position));
						p.SetLinearVelocity(glm::rotate(linearVelocity, glm::radians(emissionAngle), axis));
					}
					else p.SetLinearVelocity(linearVelocity);
					p.SetAngularVelocity(angularVelocity, angularAxis);
					p.SetLifeTime(particleLifeLength);
					p.SetColour(startColour, endColour);
					p.SetSize(startSize, endSize);
					particles.push_back(p);
				}
				accumulatedTime -= emissionRate;
			}
			for(std::list<Particle>::iterator i = particles.begin(); i != particles.end(); ) {
				(*i).Update(deltaTime);
				if((*i).IsAlive()) i++;
				else i = particles.erase(i);
			}
		}
		else started = true;
	}

	void ParticleEmitter::Render() {
		std::shared_ptr<Camera> cam = Camera::GetMainCamera().lock();
		glm::mat4 view = cam->GetView();

		shader->SetUniform("viewviewprojection", cam->GetViewProjection());
		if(texture) texture->Bind(shader);
		std::shared_ptr<Transform> camTransform = cam->GetComponent<Transform>().lock();
		glm::mat4 transform = GetComponent<Transform>().lock()->GetWorldTransform();
		glDepthMask(GL_FALSE);
		glBindVertexArray(VAO);
		for(std::list<Particle>::iterator i = particles.begin(); i != particles.end(); ++i) {
			(*i).Render(transform, camTransform->GetPosition(), camTransform->GetUp(), shader);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
	}

	void ParticleEmitter::Reset() {
		accumulatedTime = 0.0f;
		particles.clear();
	}

	void ParticleEmitter::Generate() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		vertices.push_back(ParticleVert(glm::vec3(-0.01f, -0.01f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(ParticleVert(glm::vec3(0.01f, -0.01f, 0.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(ParticleVert(glm::vec3(-0.01f, 0.01f, 0.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(ParticleVert(glm::vec3(0.01f, 0.01f, 0.0f), glm::vec2(1.0f, 0.0f)));
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(2); indices.push_back(1); indices.push_back(3);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVert) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVert), (void*)offsetof(ParticleVert, position));
		glEnableVertexAttribArray(VERTEX_POSITION);

		glVertexAttribPointer(VERTEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVert), (void*)offsetof(ParticleVert, textureCoord));
		glEnableVertexAttribArray(VERTEX_TEXCOORD);

		glVertexAttribPointer(VERTEX_COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVert), (void*)offsetof(ParticleVert, colour));
		glEnableVertexAttribArray(VERTEX_COLOUR);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	Particle::Particle() : lifeTime(0.0f), alive(true) {
	}

	Particle::~Particle() {
	}

	void Particle::SetLinearVelocity(const glm::vec3 & newLinearVelocity) {
		linearVelocity = newLinearVelocity;
		moveVector = newLinearVelocity;
	}

	const glm::vec3 Particle::GetLinearVelocity() const {
		return linearVelocity;
	}

	void Particle::SetAngularVelocity(const float & angle, const glm::vec3 & axis) {
		angularVelocity = angle; angularAxis = axis;
	}

	const float Particle::GetAngle() const {
		return angularVelocity;
	}

	const glm::vec3 Particle::GetAxis() const {
		return angularAxis;
	}

	void Particle::SetSize(const float & size) {
		startSize = size; endSize = size;
	}

	void Particle::SetSize(const float & start, const float & end) {
		startSize = start; endSize = end;
	}

	const float Particle::GetStartSize() const {
		return startSize;
	}

	const float Particle::GetEndSize() const {
		return endSize;
	}

	void Particle::SetForce(const glm::vec3 & force) {
		startForce = force; endforce = force;
	}

	void Particle::SetForce(const glm::vec3 & start, const glm::vec3 & end) {
		startForce = start; endforce = end;
	}

	void Particle::SetColour(const glm::vec4 & newColour) {
		startColour = newColour; endColour = newColour;
	}

	void Particle::SetColour(const glm::vec4 & start, const glm::vec4 & end) {
		startColour = start; endColour = end;
	}

	const glm::vec4 Particle::GetStartColour() const {
		return startColour;
	}

	const glm::vec4 Particle::GetEndColour() const {
		return endColour;
	}

	void Particle::SetLifeTime(const float & newLifeTime) {
		timeToLive = newLifeTime;
	}

	const float Particle::GetLifeTime() const {
		return lifeTime;
	}

	const bool Particle::IsAlive() const {
		return alive;
	}

	void Particle::Update(const float & deltaTime) {
		lifeTime += deltaTime;
		if(lifeTime >= timeToLive) alive = false;
		else {
			glm::vec3 forceToApply = glm::mix(startForce, endforce, glm::clamp(lifeTime / timeToLive, 0.0f, 1.0f));
			position += (moveVector + forceToApply) * deltaTime;
		}
	}

	void Particle::Render(const glm::mat4 & transform, const glm::vec3 & camPosition, const glm::vec3 & camUp, const std::shared_ptr<ShaderProgram> & shader) {
		const float noramlizedTime = glm::clamp(lifeTime / timeToLive, 0.0f, 1.0f);
		scale = glm::vec3((startSize * (1.0f - noramlizedTime)) + (endSize * noramlizedTime));
		glm::mat4 r = glm::mat4(1.0f);
		glm::vec3 relativeForward = glm::normalize(camPosition - glm::vec3(transform * glm::vec4(position, 1.0f)));
		glm::vec3 right = glm::cross(camUp, relativeForward);
		glm::vec3 up = glm::cross(relativeForward, right);
		r[0] = { right.x, right.y, right.z, 0.0f };
		r[1] = { up.x, up.y, up.z, 0.0f };
		r[2] = { relativeForward.x, relativeForward.y, relativeForward.z, 0.0f };
		glm::mat4 t = glm::scale(glm::translate(glm::mat4(1.0f), position) * r, scale);
		shader->SetUniform("model", transform * t);
		glm::vec3 startHSV = glm::hsvColor(glm::vec3(startColour)), endHSV = glm::hsvColor(glm::vec3(endColour));
		const float h = startHSV.x + (endHSV.x - startHSV.x) * noramlizedTime;
		const float s = startHSV.y + (endHSV.y - startHSV.y) * noramlizedTime;
		const float l = startHSV.z + (endHSV.z - startHSV.z) * noramlizedTime;
		glm::vec3 rgbColour = glm::rgbColor(glm::vec3(h, s, l));
		const float a = startColour.a + (endColour.a - startColour.a) * noramlizedTime;
		shader->SetUniform("colour", glm::vec4(rgbColour, a));
	}
}
