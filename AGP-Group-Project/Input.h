#pragma once
#ifndef B00289996B00227422_INPUT_H
#define B00289996B00227422_INPUT_H
#include <SDL_scancode.h>

#include <SDL_mouse.h>
#include <glm\vec2.hpp>

namespace B00289996B00227422 {
	class Input {
		friend class Engine;
	public:
		~Input();
		/// <summary>
		/// Returns true if the key was pressed during the last frame.
		/// </summary>
		/// <param name="scanCode">The scan code.</param>
		/// <returns>true if the key was pressed during the last frame, else false</returns>
		static bool GetKeyDown(SDL_Scancode scanCode);
		/// <summary>
		/// Returns true if the key was held down during the last frame.
		/// </summary>
		/// <param name="scanCode">The scan code.</param>
		/// <returns>true if the key was held down during the last frame, else false</returns>
		static bool GetKey(SDL_Scancode scanCode);
		/// <summary>
		/// Returns true if the key was released during the last frame.
		/// </summary>
		/// <param name="scanCode">The scan code.</param>
		/// <returns>true if the key was released during the last frame, else false</returns>
		static bool GetKeyUp(SDL_Scancode scanCode);
		/// <summary>
		/// Returns true if the mouse button was pressed during the last frame.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <returns>true if the mouse button was pressed during the last frame, else false</returns>
		static bool GetMouseButtonDown(int button);
		/// <summary>
		/// Returns true if the mouse button was held down during the last frame.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <returns>true if the mouse button was held down during the last frame, else false</returns>
		static bool GetMouseButton(int button);
		/// <summary>
		/// Returns true if the mouse button was released during the last frame.
		/// </summary>
		/// <param name="button">The button.</param>
		/// <returns>true if the mouse button was released during the last frame, else false</returns>
		static bool GetMouseButtonUp(int button);
		/// <summary>
		/// Gets the mouse position.
		/// </summary>
		/// <returns></returns>
		static glm::vec2 GetMousePosition();
		/// <summary>
		/// Gets the mouse movement.
		/// </summary>
		/// <returns></returns>
		static glm::vec2 GetMouseMovement();
	private:
		static void Update();
		static bool pressed[SDL_Scancode::SDL_NUM_SCANCODES]; // array of flags for key presses
		static bool changed[SDL_Scancode::SDL_NUM_SCANCODES]; // array of flags for key press changes
		static bool mousePressed[5]; // array of flags for mouse presses
		static bool mouseChanged[5]; // array of flags for mouse press changes

		static glm::vec2 mousePosition, oldMousePosition; // used to track mouse position and motion
		
	};
}

#endif // !B00289996B00227422_INPUT_H
