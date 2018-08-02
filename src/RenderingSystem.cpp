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
#include <glm\gtc\matrix_inverse.hpp>
namespace B00289996B00227422 {
	RenderingSystem::RenderingSystem() : wave(false), bloom(false), swirl(false), shadow(true), normalMapping(true), swirlTimer(0.0f), waveTimer(0.0f), collisionType(0) {
		// create depth buffers
		depthBuffer = std::make_shared<DepthBuffer>();
		
		for(size_t i = 0; i < MAX_LIGHTS; i++) {
			cubicDepthBuffers.push_back(std::make_shared<CubicDepthBuffer>());
		}
		// create frame buffers
		frameBuffer = std::make_shared<FrameBuffer>();
		for (size_t i = 0; i < 2; i++) {
			tempBuffers[i] = std::make_shared<FrameBuffer>();
		}
		// load shaders
		depthShader = FileLoader::GetInstance().LoadShader("Shaders//DepthBuffer.vert", "Shaders//DepthBuffer.frag");
		cubicDepthShader = FileLoader::GetInstance().LoadShader("Shaders//PointDepthBuffer.vert", "Shaders//PointDepthBuffer.frag", "Shaders//PointDepthBuffer.geom");
		brightShader = FileLoader::GetInstance().LoadShader("Shaders//BrightnessLimit.vert", "Shaders//BrightnessLimit.frag");
		gaussianBlur = FileLoader::GetInstance().LoadShader("Shaders//GaussianBlur.vert", "Shaders//GaussianBlur.frag");
		staticWave = FileLoader::GetInstance().LoadShader("Shaders//StaticWave.vert", "Shaders//StaticWave.frag");
		blendShader = FileLoader::GetInstance().LoadShader("Shaders//Blend.vert", "Shaders//Blend.frag");
		swirlShader = FileLoader::GetInstance().LoadShader("Shaders//Swirl.vert", "Shaders//Swirl.frag");
		// set the initial screen size
		screenWidth = Graphics::GetInstance().GetWindowWidth(), screenHeight = Graphics::GetInstance().GetWindowHeight();
	}

	unsigned int maxRenderables = 0;
	int count = 0, pCount;

	void RenderingSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
		ClearData();
		// point light count
		pCount = 0;
		for(size_t i = 0; i < gameObjects.size(); i++) {
			// store all object/components relative to the rendering process
			std::shared_ptr<GameObject> gameObject = gameObjects[i];
			if(!gameObject || !gameObject->GetEnabled()) continue;
			if(gameObject->HasComponents(COMPONENT_CAMERA)) cameraObject = gameObject;// only 1 camera is allowed
			if(gameObject->HasComponents(COMPONENT_MESH_RENDERER)) {
				allRenderables.push_back(gameObject); // all renderables, used for depth mapping
			}
			if(gameObject->HasComponents(COMPONENT_PARTICLE_EMITTER)) particleEmitters.push_back(gameObject->GetComponent<ParticleEmitter>().lock());
			if(gameObject->HasComponents(COMPONENT_LIGHT)) {
				std::shared_ptr<Light> light = gameObject->GetComponent<Light>().lock();
				if(light) { //if the light component has a light attached to it
					std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
					LightType type = light->GetLightType(); // get its type (directional/point)
					switch(type) {
					case LightType::DIRECTION_LIGHT:
						directionLightObject = gameObject;
						directionLight = light->GetLight<DirectionLight>();
						directionLight->position = transform->GetPosition();
						directionLight->direction = transform->GetForward();
						break;
					case LightType::POINT_LIGHT:
					{// if it is a point light
						std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
						if (cam) {// and a camera has been registered
							
							std::shared_ptr<PointLight> pointLight = light->GetLight<PointLight>();
							Frustum frustum = cam->GetFrustum();
							// calculate the lights bounds
							const glm::vec3 pos = transform->GetPosition();
							Sphere sphere = Sphere(pos, pointLight->lightLength);
							// add any lights that are in the cameras view to the array
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
					// check all renderable objects against the cameras frustum
					for (std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
						std::shared_ptr<GameObject> gameObject = (*i);
						std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
						std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
						OBB obb = transform->TransformOBB(meshRenderer->GetAABB());
						Sphere sphere = transform->TransformSphere(Sphere(glm::vec3(0.0f), meshRenderer->GetRadius()));
						if (frustum.CheckOBB(obb, collisionType)) {
							count++;
							renderables.push_back(*i); // adding any that are visible to the renderable list
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
		//update the timees by 2 * (the frame time)
		if(wave) waveTimer += deltaTime * 2.0f;
		if(swirl) swirlTimer += deltaTime * 2.0f;
	}

	void RenderingSystem::UpdateLightingUniforms() {
		shader->SetUniform<bool>("castShadows", shadow);
		shader->SetUniform<bool>("allowNormalMapping", normalMapping);
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
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].position").c_str(), pointLights[i]->position);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].ambient").c_str(), pointLights[i]->ambient);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), pointLights[i]->diffuse);
			shader->SetUniform<glm::vec3>(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights[i]->specular);	
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].attenuation").c_str(), pointLights[i]->attenuation);
			shader->SetUniform<float>(("pointLights[" + std::to_string(i) + "].lightLength").c_str(), pointLights[i]->lightLength);
		}
	}

	void RenderingSystem::UpdateCameraUniforms() {
		std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();
		shader->SetUniform("viewprojection", cameraObject->GetComponent<Camera>().lock()->GetViewProjection());
		std::shared_ptr<Transform> camTransform = cameraObject->GetComponent<Transform>().lock();
		shader->SetUniform("viewPosition", camTransform->GetPosition());
		glViewport(0, 0, (Graphics::GetInstance().GetWindowWidth()), (Graphics::GetInstance().GetWindowHeight()));
	}

	void RenderingSystem::ClearData() {
		// clear all data (except for the camera)
		particleEmitters.clear();
		pointLights.clear();
		spotLights.clear();
		directionLight = std::shared_ptr<DirectionLight>();
		directionLightObject = std::shared_ptr<GameObject>();
		renderables.clear();
		allRenderables.clear();
	}

	void RenderingSystem::ResizeBuffers() {
		
		unsigned int w = Graphics::GetInstance().GetWindowWidth(), h = Graphics::GetInstance().GetWindowHeight();
		if (w != screenWidth || h != screenHeight) { // if the screen size has changed since the last frame
			if (frameBuffer) {
				if (frameBuffer->GetWidth() != w || frameBuffer->GetHeight() != h) {// and the frame buffer isn't the correct size
					// resize the frame buffer to the windows size
					frameBuffer = std::make_shared<FrameBuffer>(w, h);
					tempBuffers[0] = std::make_shared<FrameBuffer>(w, h);
					tempBuffers[1] = std::make_shared<FrameBuffer>(w, h);
				}
			}
			screenWidth = w; screenHeight = h;
		}
	}

	void RenderingSystem::RenderScene() {
		ResizeBuffers();
		// clear the screen
		Graphics::GetInstance().Clear();
		// create depth maps to (potentially be used during rendering)
		std::vector<std::shared_ptr<Texture>> cubeMapTextures;
		std::shared_ptr<Texture> depthTexture;
		if (shadow) {
			//based on https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
			float near = 1.0f, far = 100.0f, halfWidth = 8.0f, halfHeight = 6.0f;
			glm::mat4 lightProjection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
			glm::mat4 lightView = glm::lookAt(directionLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 shadowMatrix = lightProjection * lightView;
			depthShader->Bind();
			depthShader->SetUniform("lightSpace", shadowMatrix);
			glDisable(GL_CULL_FACE);

			glDisable(GL_BLEND);

			GLsizei shadowWidth = (depthBuffer->GetWidth() + 0.01f), shadowHeight = GLsizei(depthBuffer->GetHeight() + 0.01f);
			glViewport(0, 0, shadowWidth, shadowHeight);
			depthBuffer->Bind();
			Graphics::GetInstance().Clear();
			// render the whole scene to a depth buffer
			if (directionLightObject) {
				for (std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
					std::shared_ptr<GameObject> gameObject = (*i);
					std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
					std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();
					std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
					depthShader->SetUniform("model", transform->GetWorldTransform());
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


			depthTexture = depthBuffer->GetTexture();

			cubeMapTextures = std::vector<std::shared_ptr<Texture>>();
			//based on https://learnopengl.com/#!Advanced-Lighting/Shadows/Point-Shadows
			if (pointLights.size() > 0) {
				
				float aspect = 1.0f;
				near = 0.0f;
				// directions for orienting shadow transform matrices
				const glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), down = glm::vec3(0.0, -1.0, 0.0), left = glm::vec3(-1.0, 0.0, 0.0),
					right = glm::vec3(1.0, 0.0, 0.0), forward = glm::vec3(0.0, 0.0, 1.0), back = glm::vec3(0.0, 0.0, -1.0);

				cubicDepthShader->Bind();

				std::shared_ptr<Camera> cam = cameraObject->GetComponent<Camera>().lock();

				for (size_t i = 0; i < pointLights.size(); i++) {
					far = pointLights[i]->lightLength; // set the far value of the perspective to the lights length
					glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);
					cubicDepthBuffers[i]->Bind(); // bind the relevant buffer
					// resize the viewport to the buffers size
					shadowWidth = (cubicDepthBuffers[i]->GetWidth() + 0.01f), shadowHeight = GLsizei(cubicDepthBuffers[i]->GetHeight() + 0.01f);
					glViewport(0, 0, shadowWidth, shadowHeight);
					Graphics::GetInstance().Clear();
					// calculate cubemap viewprojections
					glm::vec3 position = pointLights[i]->position;
					cubicDepthShader->SetUniform("lightMatrices[0]", shadowProjection * glm::lookAt(position, position + right, down));
					cubicDepthShader->SetUniform("lightMatrices[1]", shadowProjection * glm::lookAt(position, position + left, down));
					cubicDepthShader->SetUniform("lightMatrices[2]", shadowProjection * glm::lookAt(position, position + up, forward));
					cubicDepthShader->SetUniform("lightMatrices[3]", shadowProjection * glm::lookAt(position, position + down, back));
					cubicDepthShader->SetUniform("lightMatrices[4]", shadowProjection * glm::lookAt(position, position + forward, down));
					cubicDepthShader->SetUniform("lightMatrices[5]", shadowProjection * glm::lookAt(position, position + back, down));
					cubicDepthShader->SetUniform("far", far); // set the lights far value
					cubicDepthShader->SetUniform("lightPosition", position); // and its position
					// render the entire scene
					for (std::vector<std::shared_ptr<GameObject>>::iterator i = allRenderables.begin(); i != allRenderables.end(); ++i) {
						std::shared_ptr<GameObject> gameObject = (*i);
						std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
						std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();
						std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
						cubicDepthShader->SetUniform("model", transform->GetWorldTransform());
						for (std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); ++i) {
							GLuint VAO = (*i)->GetVAO();
							if (VAO) {
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
			shader->Bind();
			shader->SetUniform("lightSpace", shadowMatrix);		
		}
		shader->Bind();
		// fill all available cubmao slots in the shader
		for (int i = 0; i < MAX_LIGHTS; i++) {
			shader->SetUniform<int>(("cubeMap" + std::to_string(i)).c_str(), (3 + i));
			glActiveTexture(GL_TEXTURE3 + i);
			// if there is a visible light to use, use it
			if (shadow && i < pointLights.size()) glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextures[i]->id);
			else glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // mark it as empty
		}
		// if shadows are enabled bind the Shadows Depth map
		if (shadow) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depthTexture->id);
			shader->SetUniform("textureUnit2", 2);
		}
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);
		
		UpdateLightingUniforms();
		UpdateCameraUniforms();
		frameBuffer->Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 view = cameraObject->GetComponent<Camera>().lock()->GetView();
		// used to prevent VAOs being unnecessarily re-bound
		GLuint currentVAO = 0;
		// render all objects
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = renderables.begin(); i != renderables.end(); ++i) {
			std::shared_ptr<GameObject> gameObject = (*i);

			std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
			std::vector<std::shared_ptr<Mesh>> meshes = meshRenderer->GetMeshes();
			std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>().lock();
			shader->SetUniform("model", transform->GetWorldTransform());
			shader->SetUniform("normalMatrix", transform->GetNormalMatrix());
			std::shared_ptr<Material> material = meshRenderer->GetMaterial();
			material->Bind();
			for(std::vector<std::shared_ptr<Mesh>>::iterator j = meshes.begin(); j != meshes.end(); ++j) {
				GLuint VAO = (*j)->GetVAO();
				if (VAO != currentVAO) currentVAO = VAO;
				if (currentVAO) {
					glBindVertexArray(currentVAO);
					glDrawElements(GL_TRIANGLES, (*j)->GetIndexCount(), GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
				}
			}
		}
		// render all particle systems
		for(std::vector<std::shared_ptr<ParticleEmitter>>::iterator i = particleEmitters.begin(); i != particleEmitters.end(); ++i) {
			(*i)->Render();
		}
		// try to draw debug shapes, will do nothing if debud drawing is disabled
		PhysicsSystem::GetInstance().Draw();

		frameBuffer->Unbind();
		// begin post-processing
		std::shared_ptr <FrameBuffer> readBuffer = frameBuffer, drawBuffer = tempBuffers[0];
		if(bloom) {// based on https://learnopengl.com/#!Advanced-Lighting/Bloom
			// render the content of the frame buffer to a new frame buffer, only retaining pixels 
			// whose colour is higher than a predefined threshold
			brightShader->Bind();
			drawBuffer->Bind();
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), brightShader);

			drawBuffer->Unbind();
			// use 2 temorary buffers to repeatedly blur the resulting image
			readBuffer = tempBuffers[0];
			drawBuffer = tempBuffers[1];
			for(size_t i = 0; i < 5; i++) {
				drawBuffer->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), gaussianBlur);
				SwapFrameBuffers(drawBuffer, readBuffer);
			}

			drawBuffer->Bind();
			// finally, blend the relust with the original image (of the entire visible scene)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			blendShader->Bind();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, readBuffer->GetTexture()->id);
			blendShader->SetUniform("textureUnit1", 1);
			HUD::GetInstance().DrawFBO(frameBuffer->GetTexture(), blendShader);
			SwapFrameBuffers(drawBuffer, readBuffer);
		}
		if(wave) {// based on https://en.wikibooks.org/wiki/OpenGL_Programming/Post-Processing#A_first_effect
			drawBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			staticWave->SetUniform("offset", waveTimer);
			HUD::GetInstance().DrawFBO(readBuffer->GetTexture(), staticWave);
			drawBuffer->Unbind();
			if(readBuffer == frameBuffer) readBuffer = tempBuffers[1];
			SwapFrameBuffers(drawBuffer, readBuffer);
		}
		
		if(swirl) { // based on http://www.geeks3d.com/20110428/shader-library-swirl-post-processing-filter-in-glsl/
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
		// draw to OpenGLs default frame buffer
		frameBuffer->Unbind();
		glViewport(0, 0, (Graphics::GetInstance().GetWindowWidth()), (Graphics::GetInstance().GetWindowHeight()));
		// draw the last buffer that was  written to
		HUD::GetInstance().DrawFBO(readBuffer->GetTexture());
		// HUD text overlay.  Draws number of objects culled, number of point lights used and the number of Frames Per Second
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
	const bool RenderingSystem::GetNormalMapping() const {
		return normalMapping;
	}

	void RenderingSystem::SetNormalMapping(const bool & normalMapping) {
		this->normalMapping = normalMapping;
	}
	void RenderingSystem::SwapFrameBuffers(std::shared_ptr<FrameBuffer>& l, std::shared_ptr<FrameBuffer>& r) {
		std::shared_ptr <FrameBuffer> temp = l;
		l = r; r = temp;
	}

	bool RenderingSystem::GetFrustumCollisionType() {
		return collisionType;
	}
	void RenderingSystem::SetFrustumCollisionType(const bool & type) {
		collisionType = type;
	}
	const bool RenderingSystem::GetCastShadow() const {
		return shadow;
	}
	void RenderingSystem::SetCastShadow(const bool & castShadow) {
		shadow = castShadow;
	}
}

