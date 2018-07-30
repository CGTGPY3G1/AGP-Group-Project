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
		/// <summary> Gets the singleton instance of this class</summary>
		/// <returns>the instance</returns>
		static Graphics & GetInstance();
		/// <summary> Finalizes an instance of the <see cref="Graphics"/> class. </summary>
		~Graphics();
		/// <summary>Clears the screen.</summary>
		void Clear();
		/// <summary> calls SDL_GL_SwapWindow, replacing opengls display buffer </summary>
		void Swap();
		/// <summary>Gets the width of the window. </summary>
		/// <returns>the width of the window</returns>
		const unsigned int GetWindowWidth() const;
		/// <summary>Gets the height of the window. </summary>
		/// <returns>the height of the window</returns>
		const unsigned int GetWindowHeight() const;
		/// <summary>Gets the starting width of the window. </summary>
		/// <returns>the starting width of the window</returns>
		const unsigned int GetOriginalWindowWidth() const;
		/// <summary>Gets the starting height of the window. </summary>
		/// <returns>the starting height of the window</returns>
		const unsigned int GetOriginalWindowHeight() const;
		/// <summary>returns a vec2 detailing the scaling factor requred to scale from (original window size) to (current window size)</summary>
		/// <returns>the current windows scale (relative to it's original size)</returns>
		const glm::vec2 GetWindowScale() const;
		/// <summary> Toggles the cursor. </summary>
		void ToggleCursor();
		/// <summary> Toggles between fullscreen and windowed modes. </summary>
		void ToggleFullscreen();
	private:
		/// <summary>Initializes this instance. </summary>
		/// <param name="name">The window title.</param>
		/// <param name="screenWidth">Requested window width.</param>
		/// <param name="screenHeight">Requested window heightn.</param>
		/// <returns></returns>
		bool Init(const std::string & name = "OpenGL Engine", const unsigned int screenWidth = 1280, const unsigned int screenHeight = 720);

		/// <summary>Initializes the rendering context.</summary>
		/// <param name="name">The window title.</param>
		/// <param name="screenWidth">Requested window width.</param>
		/// <param name="screenHeight">Requested window heightn.</param>
		/// <returns></returns>
		bool InitRenderingContext(const std::string & name, const unsigned int screenWidth, const unsigned int screenHeight);
		/// <summary>Updates the size of the window.</summary>
		void UpdateWindowSize();
		/// <summary>
		/// Prevents a default instance of the <see cref="Graphics"/> class from being created.
		/// </summary>
		Graphics();
		SDL_Window * window;
		SDL_GLContext glContext;
		unsigned int originalWidth = 1, originalHeight = 1, width = 1, height = 1;
		glm::vec2 scale;
	};
}
#endif // !B00289996B00227422_GRAPHICS_H