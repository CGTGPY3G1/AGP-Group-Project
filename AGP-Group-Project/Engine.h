#pragma once
#ifndef B00289996_ENGINE_H
#define B00289996_ENGINE_H
#include <memory>
#include <GL/glew.h>
#include <SDL.h>
namespace B00289996 {
	/*class Graphics;
	class Input;
	class Time;
	class FileIO;
	class DebugDraw;*/

	class Game;
	class Engine {
	public:
		~Engine();
		static Engine & GetInstance();
		void Start();
		void Run();
		void End();
		void Close();
		bool IsRunning();
		void SetGame(std::shared_ptr<Game> newGame);
		//std::weak_ptr<Graphics> GetGraphics();
		//DebugDraw * GetDebugDraw();
		//std::weak_ptr<Input> GetInput();
		//std::weak_ptr<Time> GetTimer();
		unsigned int GetFPS();

	private:
		Engine();
		void UpdateFPS(const float & deltaTime);
		//std::shared_ptr<Graphics> graphics;
		//std::shared_ptr<Input> input;
		//std::shared_ptr<Time> timer;
		std::shared_ptr<Game> game;
		//DebugDraw * debugDraw;
		bool running;
		unsigned int fps = 0, framesThisSecond = 0;
		float fpsTimer = 0.0f, frameTimer = 0.0f, accumulator = 0.0f;

	};
}

#endif // !B00289996_ENGINE_H
