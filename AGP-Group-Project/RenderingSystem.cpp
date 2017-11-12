#include "RenderingSystem.h"
#include "Engine.h"
#include "Light.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "ParticleEmiitter.h"
#include "Shader.h"
#include "Material.h"
#include "Lights.h"
#include "Transform.h"
#include "Mesh.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"
#include "CubicDepthBuffer.h"
#include "Graphics.h"
#include "Texture.h"
#include "FileLoader.h"
#include "HUD.h"
#include <iostream>
#include "Time.h"
#include "PhysicsSystem.h"
#include "DebugDraw.h"
namespace B00289996 {
	RenderingSystem::RenderingSystem() : wave(false), bloom(false), swirl(false), swirlTimer(0.0f), waveTimer(0.0f), collisionType(0) {
		depthBuffer = std::make_shared<DepthBuffer>();
		depthShader = FileLoader::GetInstance().LoadShader("Shaders//DepthBuffer.vert", "Shaders//DepthBuffer.frag");
		for(size_t i = 0; i < MAX_LIGHTS; i++) {
			cubicDepthBuffers.push_back(std::make_shared<CubicDepthBuffer>());
		}
		cubicDepthShader = FileLoader::GetInstance().LoadShader("Shaders//PointDepthBuffer.vert", "Shaders//PointDepthBuffer.frag", "Shaders//PointDepthBuffer.geom");
		brightShader = FileLoader::GetInstance().LoadShader("Shaders//BrightnessLimit.vert", "Shaders//BrightnessLimit.frag");
		gaussianBlur = FileLoader::GetInstance().LoadShader("Shaders//GaussianBlur.vert", "Shaders//GaussianBlur.frag");
		staticWave = FileLoader::GetInstance().LoadShader("Shaders//StaticWave.vert", "Shaders//StaticWave.frag");
		blendShader = FileLoader::GetInstance().LoadShader("Shaders//Blend.vert", "Shaders//Blend.frag");
		swirlShader = FileLoader::GetInstance().LoadShader("Shaders//Swirl.vert", "Shaders//Swirl.frag");
		frameBuffer = std::make_shared<FrameBuffer>();
		for(size_t i = 0; i < 2; i++) {
			tempBuffers[i] = std::make_shared<FrameBuffer>();
		}
		glLineWidth(2);
	}

	unsigned int maxRenderables = 0;
	int count = 0, pCount;

	void RenderingSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
		ClearData();
		pCount = 0;
		for(size_t i = 0; i < gameObjects.size(); i++) {
			std::shared_ptr<GameObject> gameObject = gameObjects[i];
			if(!gameObject || !gameObject->GetEnabled()) continue;
			if(gameObject->HasComponents(COMPONENT_CAMERA)) cameraObject = gameObject;
			if(gameObject->HasComponents(COMPONENT_MESH_RENDERER)) {
				allRenderables.push_back(gameObject);
			}
			if(gameObject->HasComponents(COMPONENT_PARTICLE_EMITTER)) particleEmitters.push_back(gameObject->GetComponent<ParticleEmitter>().lock());
			if(gameObject->HasComponents(COMPONENT_LIGHT)) {
				std::shared_ptr<Light> light = gameObject->GetComponent<Light>().lock();
				std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
				if(light) {
					LightType type = light->GetLightType();
					switch(type) {
					case LightType::DIRECTION_LIGHT:
						directionLightObject = gameObject;
						directionLight = light->GetLight<DirectionLight>();
						directionLight->position = transform->GetPosition();
						directionLight->direction = transform->GetForward();
						break;
					case LightType::POINT_LIGHT:
					{
						std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
						if (cam) {
							std::shared_ptr<PointLight> pointLight = light->GetLight<PointLight>();
							
							Frustum frustum = cam->GetFrustum();
							const glm::vec3 pos = transform->GetPosition();
							Sphere sphere = Sphere(pos, pointLight->lightLength);
							if (frustum.CheckSphere(sphere, collisionType)) {
								pointLight->position = pos;
								pointLights.push_back(pointLight);
								pCount++;

							}
						}
						
					}
					break;
					default:
						break;
					}
				}
			}
		}
		maxRenderables = allRenderables.size();
		count = 0;
		if (maxRenderables > 0) {
			if (cameraObject.use_count() > 0) {
				std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
				if (cam) {
					Frustum frustum = cam->GetFrustum();
					for (std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
						std::shared_ptr<GameObject> gameObject = (*i);
						std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
						std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
						OBB obb = transform->TransformOBB(meshRenderer->GetAABB());
						
						Sphere sphere = transform->TransformSphere(Sphere(glm::vec3(0.0f), meshRenderer->GetRadius()));
						if (frustum.CheckOBB(obb, collisionType)) {
							count++;
							renderables.push_back(*i);
						//	DebugDraw::GetInstance().DrawOBB(obb, glm::vec3(1.0f, 0.0f, 0.0f));
						}

					}
				}
				
			}
			
		}
		
	}

	const unsigned int RenderingSystem::GetComponentMask() const {
		return COMPONENT_CAMERA | COMPONENT_LIGHT | COMPONENT_MESH_RENDERER;
	}

	void RenderingSystem::SetShader(std::shared_ptr<ShaderProgram> shader) {
		this->shader = shader;
	}

	void RenderingSystem::UpdateTimers(const float deltaTime) {
		if(wave) waveTimer += deltaTime * 2.0f;
		if(swirl) swirlTimer += deltaTime * 2.0f;
	}

	void RenderingSystem::UpdateLightingUniforms() {
		if(directionLight) {
			shader->SetUniform<glm::vec3>("directionLightDirection", directionLight->direction);
			shader->SetUniform<glm::vec3>("directionLight.ambient", directionLight->ambient);
			shader->SetUniform<glm::vec3>("directionLight.diffuse", directionLight->diffuse);
			shader->SetUniform<glm::vec3>("directionLight.specular", directionLight->specular);
			shader->SetUniform<float>("directionLight.intensity", directionLight->intensity);
		}

		const unsigned int noOfPointLights = pointLights.size();
		shader->SetUniform<int>("numberOfPointLights", noOfPointLights);
		for(unsigned int i = 0; i < noOfPointLights; i++) {
			shader->SetUniform<glm::vec3>(("pointLightPositions[" + std::to_string(i) + "]").c_str(), pointLights[i]->position);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].ambient").c_str(), pointLights[i]->ambient);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), pointLights[i]->diffuse);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights[i]->specular);
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].attenuation").c_str(), pointLights[i]->attenuation);
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].lightLength").c_str(), pointLights[i]->lightLength);
		}
	}

	void RenderingSystem::UpdateCameraUniforms() {
		std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
		shader->SetUniform("projection", cameraObject->GetComponent<Camera>().lock()->GetViewProjection());
		std::shared_ptr<Transform> camTransform = cameraObject->GetComponent<Transform>().lock();
		shader->SetUniform("viewPosition", camTransform->GetPosition());
		glViewport(0, 0, (Graphics::GetInstance().GetWindowWidth()), (Graphics::GetInstance().GetWindowHeight()));
	}

	void RenderingSystem::ClearData() {
		particleEmitters.clear();
		pointLights.clear();
		spotLights.clear();
		if(directionLight) directionLight = std::shared_ptr<DirectionLight>();
		directionLightObject = std::shared_ptr<GameObject>();
		renderables.clear();
		allRenderables.clear();
	}

	void RenderingSystem::ResizeBuffers() {
		unsigned int w = Graphics::GetInstance().GetWindowWidth(), h = Graphics::GetInstance().GetWindowHeight();
		if(frameBuffer) {
			if(frameBuffer->GetWidth() != w || frameBuffer->GetHeight() != h) {
				frameBuffer = std::make_shared<FrameBuffer>(w, h);
				tempBuffers[0] = std::make_shared<FrameBuffer>(w, h);
				tempBuffers[1] = std::make_shared<FrameBuffer>(w, h);
			}
		}
	}

	void RenderingSystem::RenderScene() {
		ResizeBuffers();
		Graphics::GetInstance().Clear();
		float near = 1.0f, far = 100.0f, halfWidth = 20.0f, halfHeight = 20.0f;
		glm::mat4 lightProjection = glm::ortho(-halfWidth * 2, halfWidth, -halfHeight, halfHeight, near, far);
		glm::mat4 lightView = glm::lookAt(directionLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 shadowMatrix = lightProjection * lightView;
		depthShader->Bind();
		depthShader->SetUniform("lightSpace", shadowMatrix);
		glDisable(GL_CULL_FACE);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.0f, 2.0f);
		GLsizei shadowWidth = (depthBuffer->GetWidth() + 0.01f), shadowHeight = GLsizei(depthBuffer->GetHeight() + 0.01f);
		glViewport(0, 0, shadowWidth, shadowHeight);
		depthBuffer->Bind();
		Graphics::GetInstance().Clear();
		glDisable(GL_BLEND);
		if (directionLightObject) {
			for (std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
				std::shared_ptr<GameObject> gameObject = (*i);

				std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
				std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();
				std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
				depthShader->SetUniform("modelview", transform->GetWorldTransform());
				for (std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); ++i) {
					GLuint VAO = (*i)->GetVAO();
					if (VAO) {
						glBindVertexArray(VAO);
						glDrawElements(GL_TRIANGLES, (*i)->GetIndexCount(), GL_UNSIGNED_INT, 0);
						glBindVertexArray(0);
					}
				}
			}
		}
		
		depthBuffer->Unbind();
		depthShader->UnBind();

		std::shared_ptr<Texture> depthTexture = depthBuffer->GetTexture();

		std::vector<std::shared_ptr<Texture>> cubeMapTextures = std::vector<std::shared_ptr<Texture>>();
		if(pointLights.size() > 0) {

			float aspect = 1.0f;
			near = 0.0f;
			far = 20.0f;
			const glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), down = glm::vec3(0.0, -1.0, 0.0), left = glm::vec3(-1.0, 0.0, 0.0),
				right = glm::vec3(1.0, 0.0, 0.0), forward = glm::vec3(0.0, 0.0, 1.0), back = glm::vec3(0.0, 0.0, -1.0);
			glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);


			cubicDepthShader->Bind();

			std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
			glm::mat4 camTransform = cameraObject->GetComponent<Camera>().lock()->GetViewProjection();
			for(size_t i = 0; i < pointLights.size(); i++) {
				cubicDepthBuffers[i]->Bind();
				shadowWidth = (cubicDepthBuffers[i]->GetWidth() + 0.01f), shadowHeight = GLsizei(cubicDepthBuffers[i]->GetHeight() + 0.01f);
				glViewport(0, 0, shadowWidth, shadowHeight);
				Graphics::GetInstance().Clear();
				std::vector<glm::mat4> shadowTransforms;
				glm::vec3 position = pointLights[i]->position;
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + right, down));
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + left, down));
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + up, forward));
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + down, back));
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + forward, down));
				shadowTransforms.push_back(shadowProjection * glm::lookAt(position, position + back, down));


				cubicDepthShader->SetUniform("far", far);
				for(int j = 0; j < shadowTransforms.size(); j++) {
					cubicDepthShader->SetUniform(("lightMatrices[" + std::to_string(j) + "]").c_str(), shadowTransforms[j]);
				}
				cubicDepthShader->SetUniform("lightPosition", position);
				for(std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
					std::shared_ptr<GameObject> gameObject = (*i);
					std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
					std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();
					std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
					cubicDepthShader->SetUniform("modelview", transform->GetWorldTransform());
					for(std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); ++i) {
						GLuint VAO = (*i)->GetVAO();
						if(VAO) {
							glBindVertexArray(VAO);
							glDrawElements(GL_TRIANGLES, (*i)->GetIndexCount(), GL_UNSIGNED_INT, 0);
							glBindVertexArray(0);
						}
					}
				}
				cubeMapTextures.push_back(cubicDepthBuffers[i]->GetTexture());
				cubicDepthBuffers[i]->Unbind();
			}
			cubicDepthShader->UnBind();
		}
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader->Bind();
		for(int i = 0; i < MAX_LIGHTS; i++) {
			shader->SetUniform<int>(("cubeMaps[" + std::to_string(i) + "]").c_str(), (4 + i));
			glActiveTexture(GL_TEXTURE4 + i);
			if(i < pointLights.size()) glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextures[i]->id);
			else glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		shader->SetUniform("lightSpace", shadowMatrix);
		shader->SetUniform("far", far);
		UpdateLightingUniforms();
		UpdateCameraUniforms();
		frameBuffer->Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::sort(renderables.begin(), renderables.end(), [](const std::shared_ptr<GameObject>& l, const std::shared_ptr<GameObject>& r) {
			if(l && r) {
				std::shared_ptr<MeshRenderer> lRenderer = l->GetComponent<MeshRenderer>().lock(), rRenderer = r->GetComponent<MeshRenderer>().lock();
				std::shared_ptr<Material> lMaterial = lRenderer->GetMaterial(), rMaterial = rRenderer->GetMaterial();
				return lMaterial->GetDiffuse().w > rMaterial->GetDiffuse().w;
			}
			return false;
		});

		for(std::vector<std::shared_ptr<GameObject>>::iterator i = renderables.begin(); i != renderables.end(); ++i) {
			std::shared_ptr<GameObject> gameObject = (*i);
			std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
			std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();

			std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
			shader->SetUniform("modelview", transform->GetWorldTransform());
			shader->SetUniform("normalMatrix", transform->GetNormalMatrix());
			std::shared_ptr<Material> material = meshRenderer->GetMaterial();
			material->ReplaceTexture(TextureType::DEPTH_MAP, depthTexture);
			material->Bind();
			const bool TRANSPARENT = material->GetDiffuse().w < 0.9999f;
			if(TRANSPARENT) glDepthMask(GL_FALSE);
			for(std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); ++i) {
				GLuint VAO = (*i)->GetVAO();
				if(VAO) {
					glBindVertexArray(VAO);
					glDrawElements(GL_TRIANGLES, (*i)->GetIndexCount(), GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
				}
			}
			if(TRANSPARENT) glDepthMask(GL_TRUE);
		}

		for(std::vector<std::shared_ptr<ParticleEmitter>>::iterator i = particleEmitters.begin(); i != particleEmitters.end(); ++i) {
			(*i)->Render();
		}

		PhysicsSystem::GetInstance().Draw();

		frameBuffer->Unbind();
		std::shared_ptr <FrameBuffer> readBuffer = frameBuffer, drawBuffer = tempBuffers[0];
		if(bloom) {
			brightShader->Bind();
			drawBuffer->Bind();
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), brightShader);
			HUD::GetInstance().Render();
			drawBuffer->Unbind();
			readBuffer = tempBuffers[0];
			drawBuffer = tempBuffers[1];

			for(size_t i = 0; i < 5; i++) {
				drawBuffer->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), gaussianBlur);
				SwapFrameBuffers(drawBuffer, readBuffer);
			}

			drawBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			blendShader->Bind();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, readBuffer->GetTexture()->id);
			blendShader->SetUniform("textureUnit1", 1);
			HUD::GetInstance().DrawFBO(frameBuffer->GetTexture(), blendShader);
			SwapFrameBuffers(drawBuffer, readBuffer);
		}
		if(wave) {
			drawBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			staticWave->SetUniform("offset", waveTimer);
			HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), staticWave);
			drawBuffer->Unbind();
			if(readBuffer == frameBuffer) readBuffer = tempBuffers[1];
			SwapFrameBuffers(drawBuffer, readBuffer);
		}
		
		if(swirl) {
			drawBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			staticWave->Bind();
			swirlShader->SetUniform("time", swirlTimer);
			glm::vec2 centre = glm::vec2((float)drawBuffer->GetWidth() / 2.0f, (float)drawBuffer->GetHeight() / 2.0f);
			swirlShader->SetUniform("centre", centre);
			HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), swirlShader);
			if(readBuffer == frameBuffer) readBuffer = tempBuffers[1];
			SwapFrameBuffers(drawBuffer, readBuffer);
		}
		
		frameBuffer->Unbind();
		glViewport(0, 0, (Graphics::GetInstance().GetWindowWidth()), (Graphics::GetInstance().GetWindowHeight()));
		
		HUD::GetInstance().DrawFBO(readBuffer->GetTexture());
		HUD::GetInstance().DrawText("Culled = " + std::to_string(maxRenderables - count) + "/" + std::to_string(maxRenderables), -0.9f, 0.85f, 0.05f, Alignment::Left);
		if(pointLights.size() > 0) HUD::GetInstance().DrawText("Used Lights = " + std::to_string(pCount), -0.9f, 0.78f, 0.05f, Alignment::Left);
		HUD::GetInstance().DrawText( std::to_string(Engine::GetInstance().GetFPS()) + " FPS", 0.9f, 0.85f, 0.05f, Alignment::Right);
		

		
		HUD::GetInstance().Render();
		
	}

	const bool RenderingSystem::GetWave() const {
		return wave;
	}
	void RenderingSystem::SetWave(const bool & wave) {
		if(wave) waveTimer = 0.0f;
		this->wave = wave;
	}
	const bool RenderingSystem::GetBlur() const {
		return blur;
	}

	void RenderingSystem::SetBlur(const bool & blur) {
		this->blur = blur;
	}

	const bool RenderingSystem::GetBloom() const {
		return bloom;
	}
	void RenderingSystem::SetBloom(const bool & bloom) {
		this->bloom = bloom;
	}
	const bool RenderingSystem::GetSwirl() const {
		return swirl;
	}
	void RenderingSystem::SetSwirl(const bool & swirl) {
		if(swirl) swirlTimer = 0.0f;
		this->swirl = swirl;
	}
	void RenderingSystem::SwapFrameBuffers(std::shared_ptr<FrameBuffer>& l, std::shared_ptr<FrameBuffer>& r) {
		std::shared_ptr <FrameBuffer> temp = l;
		l = r; r = temp;
	}

	int RenderingSystem::GetFrustumCollisionType() {
		return collisionType;
	}
	void RenderingSystem::SetFrustumCollisionType(const int & type) {
		collisionType = type;
	}
}

