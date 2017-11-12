#include "Game.h"
#include "FileLoader.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "ComponentHeaders.h"
#include "Shader.h"
#include "Material.h"
#include "Graphics.h"
#include "Input.h"
#include <iostream>
#include "RenderingSystem.h"
#include "ScriptManagementSystem.h"
#include "Lights.h"
#include "HUD.h"
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include "PhysicsSystem.h"
namespace B00289996 {
	Game::Game() : lastVelocity(glm::vec3(0.0f, 0.0f, 1.0f)){
		// Load Main Shader + Rendering/Script Management Systems
		scriptManagementSystem = std::make_shared<ScriptManagementSystem>();
		renderingSystem = std::make_shared<RenderingSystem>();
		shader = FileLoader::GetInstance().LoadShader("Shaders//AllInOne.vert", "Shaders//AllInOne.frag");
		shader->Bind();
		renderingSystem->SetShader(shader);
		hudShader = FileLoader::GetInstance().LoadShader("Shaders//textured.vert", "Shaders//textured.frag");
		HUD::GetInstance().SetShader(hudShader);
		//Camera
		std::shared_ptr<GameObject> cam = GameObjectManager::GetInstance().CreateGameObject("Camera").lock();
		cam->AddComponent<Camera>().lock()->SetAsMain();
		cam->GetComponent<Transform>().lock()->SetPosition(0.0f, 1.0f, -4.0f);

		//Directional Light
		std::shared_ptr<GameObject> directionLight = GameObjectManager::GetInstance().CreateGameObject("DirectionLight").lock();
		std::shared_ptr<Light> dl = directionLight->AddComponent<Light>().lock();
		std::shared_ptr<DirectionLight> l = dl->GetLight<DirectionLight>();
		l->diffuse = glm::vec3(1.0f, 0.95f, 0.84f);
		l->ambient = glm::vec3(1.0f, 0.95f, 0.84f);
		l->specular = glm::vec3(0.2f, 0.2f, 0.2f);
		l->intensity = 0.8f;

		std::shared_ptr<Transform> directionLightTransform = directionLight->GetComponent<Transform>().lock();
		if (directionLightTransform) {
			directionLightTransform->SetPosition(20.0f, 20.0f, 20.0f);
			directionLightTransform->SetForward(-directionLightTransform->GetPosition());
		}
		std::shared_ptr<GameObject> directionLightBase = GameObjectManager::GetInstance().CreateGameObject("DirectionLightBase").lock();
		std::shared_ptr<Transform> directionLightBaseTransform = directionLightBase->GetComponent<Transform>().lock();
		directionLightTransform->SetParent(directionLightBaseTransform);

		//Particles
		//Fire
		std::shared_ptr<GameObject> campFire = GameObjectManager::GetInstance().CreateGameObject("Campfire").lock();
		std::shared_ptr<Transform> campFireTransform = campFire->GetComponent<Transform>().lock();
		campFireTransform->SetPosition(1.5f, 0.0f, 0.0f);
		std::shared_ptr<MeshRenderer> fireMesh = campFire->AddComponent<MeshRenderer>().lock();
		fireMesh->AddMeshes(FileLoader::GetInstance().LoadMeshes("OBJModels//rocks.obj", true));
		std::shared_ptr<Material> rockMaterial = std::make_shared<Material>();
		rockMaterial->SetShader(shader);
		rockMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//rocks_diffuse.png", DIFFUSE));
		rockMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//rocks_normal.png", NORMAL_MAP));
		fireMesh->SetMaterial(rockMaterial);
		std::shared_ptr<RigidBody> rigidbody = campFire->AddComponent<RigidBody>().lock();
		rigidbody->Init(false);
		rigidbody->AttachCylinder(glm::vec3(0.0f, 0.15f, 0.0f), glm::vec3(0.2f, 0.3f, 0.2f));

		std::shared_ptr<GameObject> particleEmitter1 = GameObjectManager::GetInstance().CreateGameObject("Fire").lock();
		std::shared_ptr<Transform> fireTransform = particleEmitter1->GetComponent<Transform>().lock();
		fireTransform->SetParent(campFireTransform);
		fireTransform->SetLocalPosition(0.0f, -0.1f, 0.0f);

		std::shared_ptr<ParticleEmitter> fireParticles = particleEmitter1->AddComponent<ParticleEmitter>().lock();
		fireParticles->SetParticleLifeTime(1.0f);
		fireParticles->SetEmissionRate(0.002f);
		fireParticles->SetLinearVelocity(glm::vec3(0.0f, 0.5f, 0.0f));
		fireParticles->SetSize(7.0f, 0.01f);
		fireParticles->SetEmissionAngle(-2.0f);
		fireParticles->SetColour(glm::vec4(0.7f, 0.15f, 0.04f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 0.2f));
		fireParticles->SetTexture(FileLoader::GetInstance().LoadTexture("Textures//Fire.png"));
		//Smoke
		std::shared_ptr<GameObject> particleEmitter2 = GameObjectManager::GetInstance().CreateGameObject("Smoke").lock();
		std::shared_ptr<Transform> smokeTransform = particleEmitter2->GetComponent<Transform>().lock();
		smokeTransform->SetPosition(fireTransform->GetPosition() + glm::vec3(0.0f, 0.35f, 0.0f));
		smokeTransform->SetParent(fireTransform);
		std::shared_ptr<ParticleEmitter> smokeParticles = particleEmitter2->AddComponent<ParticleEmitter>().lock();
		smokeParticles->SetParticleLifeTime(20.0f);
		smokeParticles->SetEmissionRate(0.05f);
		smokeParticles->SetLinearVelocity(glm::vec3(0.0, 0.25, 0.0));
		smokeParticles->SetSize(7.5f, 80.0f);
		smokeParticles->SetEmissionAngle(8.0f);
		smokeParticles->SetRadius(0.09f);
		smokeParticles->SetColour(glm::vec4(0.5f, 0.5f, 0.5f, 0.3f), glm::vec4(0.15f, 0.15f, 0.15f, 0.0f));
		smokeParticles->SetStartDelay(0.5f);
		smokeParticles->SetTexture(FileLoader::GetInstance().LoadTexture("Textures//Particle.png"));
		//Sparks
		std::shared_ptr<GameObject> particleEmitter3 = GameObjectManager::GetInstance().CreateGameObject("Sparks").lock();
		std::shared_ptr<Transform> sparkTransform = particleEmitter3->GetComponent<Transform>().lock();
		sparkTransform->SetPosition(fireTransform->GetPosition() + glm::vec3(0.0f, 0.15f, 0.0f));
		sparkTransform->SetParent(fireTransform);
		std::shared_ptr<ParticleEmitter> sparkParticles = particleEmitter3->AddComponent<ParticleEmitter>().lock();
		sparkParticles->SetParticleLifeTime(0.8f);
		sparkParticles->SetEmissionRate(0.25f);
		sparkParticles->SetLinearVelocity(glm::vec3(0.0, 0.3, 0.0));
		sparkParticles->SetSize(1.2f, 0.5f);
		sparkParticles->SetEmissionAngle(20.0f);
		sparkParticles->SetRadius(0.1f);
		sparkParticles->SetColour(glm::vec4(0.8f, 0.32f, 0.0f, 0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 0.1f));
		smokeParticles->SetTexture(FileLoader::GetInstance().LoadTexture("Textures//Particle.png"));

		std::shared_ptr<GameObject> fireLight = GameObjectManager::GetInstance().CreateGameObject("FireLight").lock();
		std::shared_ptr<Transform> fireLightTransform = fireLight->GetComponent<Transform>().lock();
		fireLightTransform->SetPosition(sparkTransform->GetPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
		fireLightTransform->SetParent(fireTransform);
		std::shared_ptr<Light> fl = fireLight->AddComponent<Light>().lock();
		fl->SetLightType(LightType::POINT_LIGHT);
		std::shared_ptr<PointLight> fl1 = fl->GetLight<PointLight>();
		fl1->diffuse = glm::vec3(1.0f, 0.7f, 0.45f);
		fl1->ambient = glm::vec3(1.0f, 0.7f, 0.45f);
		fl1->specular = glm::vec3(0.7f, 0.3f, 0.1f);
		fl1->attenuation = 0.8f;
		fl1->lightLength = 4.0f;
		fireLight->AddComponent<LightFlicker>();
		campFire->SetEnabled(false);

		

		//Point Lights
		const float angle = 45.0f;
		float sX = -5.0f, sZ = -6.5f;
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++) {
				//Set-Up Rotating Parent Objects
				std::shared_ptr<GameObject> rotator1 = GameObjectManager::GetInstance().CreateGameObject("BaseRotator").lock();
				std::shared_ptr<AutoRotator> autoRotatorScript1 = rotator1->AddComponent<AutoRotator>().lock();
				autoRotatorScript1->SetValues(glm::vec3(0.0f, 1.0f, 0.0f), (2.0f * ((j % 2) == ((i % 2) == 1) ? 1 : 0) ? angle : -angle));
				std::shared_ptr<Transform> rotatorTransform1 = rotator1->GetComponent<Transform>().lock();
				std::shared_ptr<GameObject> rotator2 = GameObjectManager::GetInstance().CreateGameObject("Rotator").lock();
				std::shared_ptr<AutoRotator> autoRotatorScript2 = rotator2->AddComponent<AutoRotator>().lock();
				autoRotatorScript2->SetValues(glm::vec3(0.0f, 1.0f, 0.0f), ((j % 1) == ((i % 2) == 1) ? 1 : 0) ? -angle : angle);
				std::shared_ptr<Transform> rotatorTransform2 = rotator2->GetComponent<Transform>().lock();

				std::shared_ptr<GameObject> pointLight = GameObjectManager::GetInstance().CreateGameObject("PointLight").lock();
				std::shared_ptr<Transform> pointLightTransform = pointLight->GetComponent<Transform>().lock();
				//Set-Up Lights
				float x = sX + 5.0f * i, y = 1.0f, z = sZ + 5.0f * j;
				pointLightTransform->SetPosition(x, y, z);
				z += (j % 1 == 0) ? 1.0f : -1.0f;
				rotatorTransform1->SetPosition(x, y, z);
				z += (j % 1 == 1) ? 1.5f : -1.5f;

				//Move Rotator2 and Set Rotator1 as it's parent parent
				rotatorTransform2->SetPosition(x, y, z);
				rotatorTransform2->SetParent(rotatorTransform1);
				pointLightTransform->SetParent(rotatorTransform2);
				//std::cout << glm::to_string(pointLightTransform->GetPosition()) << std::endl;
				std::shared_ptr<Light> pl = pointLight->AddComponent<Light>().lock();
				pl->SetLightType(LightType::POINT_LIGHT);
				std::shared_ptr<PointLight> l2 = pl->GetLight<PointLight>();
				const unsigned int val = (i + j) % 3;
				if (val == 0) {
					l2->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
					l2->ambient = glm::vec3(1.0f, 0.0f, 0.0f);
					l2->specular = glm::vec3(0.2f, 0.2f, 0.2f);
				}
				else if (val == 1) {
					l2->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
					l2->ambient = glm::vec3(0.0f, 1.0f, 0.0f);
					l2->specular = glm::vec3(0.2f, 0.2f, 0.2f);
				}
				else {
					l2->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
					l2->ambient = glm::vec3(0.0f, 0.0f, 1.0f);
					l2->specular = glm::vec3(0.2f, 0.2f, 0.2f);
				}
				l2->attenuation = 0.8f;
				l2->lightLength = 5.0f;
			}
		}

		//Models
		const float startX = -9.0f, startZ = -9.0f;
		for (size_t a = 0; a < 19; a++) {
			for (size_t i = 0; i < 19; i++) {
				std::shared_ptr<GameObject> cube = GameObjectManager::GetInstance().CreateGameObject("Cube").lock();
				cube->GetComponent<Transform>().lock()->SetPosition(startX + i, 0.1f, startZ + a);
				cube->GetComponent<Transform>().lock()->Scale(0.1f);
				std::shared_ptr<MeshRenderer> meshRenderer = cube->AddComponent<MeshRenderer>().lock();
				meshRenderer->AddMeshes(FileLoader::GetInstance().LoadMeshes("OBJModels//cube.obj", true));
				std::shared_ptr<Material> m = std::make_shared<Material>();
				m->SetShader(shader);
				m->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//crate1_diffuse.bmp", DIFFUSE));
				m->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//crate1_normal.bmp", NORMAL_MAP));
				meshRenderer->SetMaterial(m);
				std::shared_ptr<RigidBody> rigidbody = cube->AddComponent<RigidBody>().lock();
				rigidbody->Init(false);
				rigidbody->AttachBox(glm::vec3(0.0f), glm::vec3(0.2f));
			}
		}


		//Set-Up Ground Quad
		std::shared_ptr<GameObject> ground = GameObjectManager::GetInstance().CreateGameObject("Ground").lock();
		std::shared_ptr<MeshRenderer> groundRenderer = ground->AddComponent<MeshRenderer>().lock();
		groundRenderer->AddMeshes(FileLoader::GetInstance().LoadMeshes("OBJModels//ground.obj", false));
		groundRenderer->GetMesh(0)->ScaleUVs(100.0f);
		std::shared_ptr<Material> groundMaterial = std::make_shared<Material>();
		groundMaterial->SetShader(shader);
		groundMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//pavement_diffuse.bmp", DIFFUSE));
		groundMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//pavement_normal.bmp", NORMAL_MAP));
		groundRenderer->SetMaterial(groundMaterial);
		ground->GetComponent<Transform>().lock()->Scale(10.0f);
		std::shared_ptr<RigidBody> groundRigidBody = ground->AddComponent<RigidBody>().lock();
		groundRigidBody->Init(false);
		groundRigidBody->AttachBox(glm::vec3(0.0f), glm::vec3(20.0f, 0.001f, 20.0f), 0.0f);

		//Set-Up Player
		std::shared_ptr<GameObject> player = GameObjectManager::GetInstance().CreateGameObject("Player").lock();
		std::shared_ptr<MeshRenderer> playerRenderer = player->AddComponent<MeshRenderer>().lock();
		playerRenderer->AddMeshes(FileLoader::GetInstance().LoadMeshes("OBJModels//sphere.obj", false));
		std::shared_ptr<Material> sphereMaterial = std::make_shared<Material>();
		sphereMaterial->SetShader(shader);
		sphereMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//earth_diffuse.bmp", DIFFUSE));
		sphereMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//earth_normal.bmp", NORMAL_MAP));
		playerRenderer->SetMaterial(sphereMaterial);
		player->GetComponent<Transform>().lock()->SetPosition(1.0f, 0.2f, -0.5f);
		player->GetComponent<Transform>().lock()->Scale(0.1f);
		std::shared_ptr<RigidBody> playerBody = player->AddComponent<RigidBody>().lock();
		playerBody->Init(true, 0.05f, 0.01f);
		playerBody->AttachSphere(glm::vec3(0.0f), 0.1f, 10.0f);

		ToggleDirectionalLight();
		TogglePointLights();

		std::shared_ptr<GameObject> wall = CreateWall(glm::vec3(0.0f, 0.0f, 10.0f));
		wall = CreateWall(glm::vec3(0.0f, 0.0f, -10.0f));
		wall = CreateWall(glm::vec3(-10.0f, 0.0f, 0.0f), 90.0f);

		wall = CreateWall(glm::vec3(10.0f, 0.0f, 0.0f), 90.0f);

		std::shared_ptr<MeshRenderer> wallRenderer = wall->AddComponent<MeshRenderer>().lock();
		wallRenderer->GetMesh(0)->ScaleUVs(2.0f);
	}

	Game::~Game() {

	}
	bool moveFor = true;
	void Game::Update(const float & deltaTime) {
		//Process Frame Time Updates
		if(Input::GetKeyDown(SDL_SCANCODE_F)) Graphics::GetInstance().ToggleFullscreen();
		if(Input::GetKeyDown(SDL_SCANCODE_C)) Graphics::GetInstance().ToggleCursor();
		
		if(Input::GetKeyDown(SDL_SCANCODE_1)) ToggleDirectionalLight(); //Toggle Directional Lighting On/Off
		if(Input::GetKeyDown(SDL_SCANCODE_2)) TogglePointLights(); //Toggle Point Lighting On/Off
		//Toggle Particle System On/Off
		if(Input::GetKeyDown(SDL_SCANCODE_3)) {
			std::shared_ptr<GameObject> campFire = GameObjectManager::GetInstance().GetGameObject("Campfire").lock();
			campFire->SetEnabled(!campFire->GetEnabled());
		}
		// Toggle Post Processing Effects
		if(Input::GetKeyDown(SDL_SCANCODE_4)) renderingSystem->SetBloom(!renderingSystem->GetBloom());
		if(Input::GetKeyDown(SDL_SCANCODE_5)) renderingSystem->SetWave(!renderingSystem->GetWave());
		if(Input::GetKeyDown(SDL_SCANCODE_6)) renderingSystem->SetSwirl(!renderingSystem->GetSwirl());
		// Toggle Debug Draw
		if (Input::GetKeyDown(SDL_SCANCODE_I)) PhysicsSystem::GetInstance().SetDrawDebug(!PhysicsSystem::GetInstance().GetDrawDebug());

		// Switch Frustum culling method 
		if (Input::GetKeyDown(SDL_SCANCODE_T)) renderingSystem->SetFrustumCollisionType(renderingSystem->GetFrustumCollisionType() == 1 ? 0 : 1);
		std::shared_ptr<GameObject> player = GameObjectManager::GetInstance().GetGameObject("Player").lock();
		std::shared_ptr<RigidBody> playerRb = player->GetComponent<RigidBody>().lock();
		if (Input::GetKeyDown(SDL_SCANCODE_R)) {
			playerRb->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
		}

		//Move The Camera 
		std::shared_ptr<GameObject> camera = GameObjectManager::GetInstance().GetGameObject("Camera").lock();
		std::shared_ptr<Transform> camTransform = camera->GetComponent<Transform>().lock();
		// spherically interpolate the cameras orientation to face the target
		std::shared_ptr<Transform> target = player->GetComponent<Transform>().lock();
		camTransform->SlerpRotation(camTransform->LookAt(target->GetPosition(), camTransform->GetPosition()), 2.0f * deltaTime);
		// spherically interpolate the cameras position to the target point
		glm::vec3 velocity = playerRb->GetLinearVelocity();
		velocity.y = 0.0f;
		if (glm::length2(velocity) > 0.01) lastVelocity = glm::normalize(velocity);

		glm::vec3 lerpTo = target->GetPosition() + glm::vec3(0.0f, 0.4f, 0.0f) + (lastVelocity * -1.5f);
		camTransform->LerpPosition(lerpTo, deltaTime);

		// Process Scriptable Components
		std::vector<std::shared_ptr<GameObject>> gameObjects = GameObjectManager::GetInstance().GetGameObjects();
		scriptManagementSystem->LoadScripts(gameObjects);
		scriptManagementSystem->Update(deltaTime);
		// Update Post Processing Effect timers
		renderingSystem->UpdateTimers(deltaTime);
		// Add Components to rendering system
		renderingSystem->ProcessComponents(gameObjects);
	}

	void Game::FixedUpdate(const float & fixedDeltaTime) {
		scriptManagementSystem->FixedUpdate(fixedDeltaTime);
		std::shared_ptr<GameObject> player = GameObjectManager::GetInstance().GetGameObject("Player").lock();
		std::shared_ptr<RigidBody> playerRB = player->GetComponent<RigidBody>().lock();
		std::shared_ptr<GameObject> camera = GameObjectManager::GetInstance().GetGameObject("Camera").lock();
		std::shared_ptr<Transform> camTransform = camera->GetComponent<Transform>().lock();
		glm::vec3 forward = camTransform->GetForward(), right = camTransform->GetRight();
		forward.y = right.y = 0.0f;
		forward = glm::normalize(forward) * 0.1f;
		right = glm::normalize(right) * 0.1f;
		if (Input::GetKey(SDL_SCANCODE_W)) playerRB->AddForce(forward, true);
		if (Input::GetKey(SDL_SCANCODE_A)) playerRB->AddForce(right, true);
		if (Input::GetKey(SDL_SCANCODE_D)) playerRB->AddForce(-right, true);
		if (Input::GetKey(SDL_SCANCODE_S)) playerRB->AddForce(-forward, true);
	}

	void Game::Render() {
		renderingSystem->RenderScene();
		
		scriptManagementSystem->Render();
		
		Graphics::GetInstance().Swap();
	}

	const bool Game::GameOver() const {
		return gameOver;
	}

	void Game::TogglePointLights() {
		std::vector<std::shared_ptr<GameObject>> lights = GameObjectManager::GetInstance().GetGameObjectsWithName("BaseRotator");
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = lights.begin(); i != lights.end(); ++i) {
			if(*i) (*i)->SetEnabled(!(*i)->GetEnabled());
		}
	}

	void Game::ToggleDirectionalLight() {
		directionalLightEnabled = !directionalLightEnabled;
		shader->SetUniform("directionalLightEnabled", directionalLightEnabled);
	}

	void Game::SetRunning(const bool & shouldRun) {
		if(running != shouldRun) running = shouldRun;
	}

	const bool Game::IsPaused() const {
		return !running;
	}

	std::shared_ptr<GameObject> Game::CreateWall(const glm::vec3 & position, const float & angle) {
		std::shared_ptr<GameObject> wall = GameObjectManager::GetInstance().CreateGameObject("Wall").lock();
		std::shared_ptr<MeshRenderer> wallRenderer = wall->AddComponent<MeshRenderer>().lock();
		wallRenderer->AddMeshes(FileLoader::GetInstance().LoadMeshes("OBJModels//wall.obj", false));
		std::shared_ptr<Material> wallMaterial = std::make_shared<Material>();

		wallMaterial->SetShader(shader);
		wallMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//wall_diffuse.png", DIFFUSE));
		wallMaterial->AddTexture(FileLoader::GetInstance().LoadTexture("Textures//wall_normal.png", NORMAL_MAP));
		wallRenderer->SetMaterial(wallMaterial);
		wall->GetComponent<Transform>().lock()->SetPosition(position);
		wall->GetComponent<Transform>().lock()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), angle);
		std::shared_ptr<RigidBody> wallRigidBody = wall->AddComponent<RigidBody>().lock();
		wallRigidBody->Init(false);
		wallRigidBody->AttachBox(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(20.0f, 1.0, 0.1f), 0.0f);
		return wall;
	}

}
