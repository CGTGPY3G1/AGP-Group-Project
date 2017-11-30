#pragma once
#ifndef B00289996B00227422_GRAPHICS_H
#define B00289996B00227422_GRAPHICS_H
#include <GL/glew.h>
#include <SDL.h>
#include <string>
#include <glm\vec2.hpp>
namespace B00289996B00227422 {
	class Graphics {
		friend class Engine;
	public:
		static Graphics & GetInstance();
		~Graphics();
		void Clear();
		void Swap();
		const unsigned int GetWindowWidth() const;
		const unsigned int GetWindowHeight() const;
		const unsigned int GetOriginalWindowWidth() const;
		const unsigned int GetOriginalWindowHeight() const;
		const glm::vec2 GetWindowScale() const;
		void ToggleCursor();
		void ToggleFullscreen();
	private:
		bool Init(const std::string & name = "OpenGL Engine", const unsigned int screenWidth = 1280, const unsigned int screenHeight = 720);
		bool InitRenderingContext(const std::string & name, const unsigned int screenWidth, const unsigned int screenHeight);
		void UpdateWindowSize();
		Graphics();
		SDL_Window * window;
		SDL_GLContext glContext;
		unsigned int originalWidth = 1, originalHeight = 1, width = 1, height = 1;
		glm::vec2 scale;
	};
}
#endif // !B00289996B00227422_GRAPHICS_H