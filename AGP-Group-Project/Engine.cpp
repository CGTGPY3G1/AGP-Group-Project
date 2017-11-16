#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_events.h>
#include "Graphics.h"
#include "Timer.h"
#include "Input.h"
#include <glm\vec4.hpp>
#include <glm\common.hpp>
#include "Game.h"
#include "PhysicsSystem.h"
#include "FileLoader.h"

/*#include "DebugDraw.h"
#include "AssetManager.h"*/
namespace B00289996 {
	Engine::Engine() {
		Graphics::GetInstance().Init();
		running = true;
	}

	void Engine::UpdateFPS(const float & deltaTime) {
		frameTimer += deltaTime;
		framesThisSecond++;
		if (frameTimer >= 1.0f) {
			fps = 0;
			int count = 0;
			do {
				count++;
				fps += framesThisSecond;
				frameTimer -= 1.0f;
				framesThisSecond = 0;
			} while (frameTimer >= 1.0f);
			fps /= count;
			std::cout << "FPS = " << fps << std::endl;
		}
	}

	Engine::~Engine() {
	}

	Engine & Engine::GetInstance() {
		static Engine instance;
		return instance;
	}

	void Engine::Start() {
		running = true;
	}

	void Engine::Run() {
		SDL_Event events;
		while (SDL_PollEvent(&events)) {
			if (events.type == SDL_QUIT) {
				running = false;
				return;
			}
			else if (events.type == SDL_WINDOWEVENT) {
				switch (events.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					Graphics::GetInstance().UpdateWindowSize();
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					game->SetRunning(true);
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					game->SetRunning(false);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					End();
					break;
				default:

					break;
				}
			}
		}
		Input::Update();
		Timer::Update();

		const float deltaTime = Timer::GetDeltaTime();
		const float fixedDeltaTime = Timer::GetFixedDeltaTime();
		//Pause/Unpuse The Scene
		if (Input::GetKeyDown(SDL_SCANCODE_ESCAPE)) End();
		else {
			if (Input::GetKeyDown(SDL_SCANCODE_RETURN)) game->SetRunning(game->IsPaused());
			if (game && !game->GameOver() && !game->IsPaused()) {
				accumulator += deltaTime;

				while (accumulator >= fixedDeltaTime) {
					PhysicsSystem::GetInstance().Update(fixedDeltaTime); // step the physics simulation
					if (game && !game->GameOver()) game->FixedUpdate(fixedDeltaTime);
					accumulator -= fixedDeltaTime; // decrement the accumulator
				}
				game->Update(deltaTime);
				game->Render();
			}

			UpdateFPS(deltaTime);
		}
	}

	void Engine::End() {
		running = false;
	}

	void Engine::Close() {
		End();		
	}

	bool Engine::IsRunning() {
		return running;
	}

	void Engine::SetGame(std::shared_ptr<Game> newGame) {
		game = newGame;
	}

	unsigned int Engine::GetFPS() {
		return fps;
	}

}