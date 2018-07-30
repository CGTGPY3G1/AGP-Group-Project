#include "Graphics.h"

#include <iostream>
namespace B00289996B00227422 {

	bool Graphics::Init(const std::string & name, const unsigned int screenWidth, const unsigned int screenHeight){
		if(!InitRenderingContext(name, screenWidth, screenHeight)) return false;
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) {
			std::cout << "glewInit failed." << std::endl;
			return false;
		}
		originalWidth = screenWidth; originalHeight = screenHeight;
		width = screenWidth; height = screenHeight;
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return true;
	}

	bool Graphics::InitRenderingContext(const std::string & name, const unsigned int screenWidth, const unsigned int screenHeight) {
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "Initialization Failed" << std::endl;
			return false;
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8); // Turn on x8 multisampling anti-aliasing (MSAA)
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								  screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if(!window) {
			std::cout << "Window not created" << std::endl;
			return false;
		}
		glContext = SDL_GL_CreateContext(window);
		if(glContext == NULL) {
			std::cout << "Rendering context not created" << std::endl;
			return false;
		}
		SDL_SetWindowResizable(window, SDL_TRUE);
		
		SDL_GL_SetSwapInterval(0);
		return true;
	}

	void Graphics::UpdateWindowSize() {
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		scale.x = (float)w / (float)originalWidth; scale.y = (float)h / (float)originalHeight;
		glViewport(0, 0, w, h);
		width = w; height = h;
	}

	Graphics::Graphics() : scale(glm::vec2(1.0f, 1.0f)) {
		
	}

	Graphics & Graphics::GetInstance() {
		static Graphics instance;
		return instance;
	}

	Graphics::~Graphics() {
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void Graphics::Clear() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Graphics::Swap() {
		SDL_GL_SwapWindow(window);
	}

	const unsigned int Graphics::GetWindowWidth() const {
		return width;
	}

	const unsigned int Graphics::GetWindowHeight() const {
		return height;
	}

	const unsigned int Graphics::GetOriginalWindowWidth() const {
		return originalWidth;
	}

	const unsigned int Graphics::GetOriginalWindowHeight() const {
		return originalHeight;
	}

	const glm::vec2 Graphics::GetWindowScale() const {
		return scale;
	}

	void Graphics::ToggleCursor() {
		SDL_ShowCursor((SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) ? SDL_ENABLE : SDL_DISABLE);
	}

	void Graphics::ToggleFullscreen() {
		SDL_SetWindowFullscreen(window, SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		width = w; height = h;
	}
}
