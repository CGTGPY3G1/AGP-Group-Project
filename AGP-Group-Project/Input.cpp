#include "Input.h"
#include "SDL.h"
namespace B00289996 {

	bool Input::pressed[SDL_Scancode::SDL_NUM_SCANCODES] = {false};
	bool Input::changed[SDL_Scancode::SDL_NUM_SCANCODES] = {false};
	bool Input::mousePressed[5] = {false};
	bool Input::mouseChanged[5] = {false};
	glm::vec2 Input::mousePosition = glm::vec2(), Input::oldMousePosition = glm::vec2();

	Input::~Input() {
	}

	bool Input::GetKeyDown(SDL_Scancode scanCode) {
		return pressed[scanCode] && changed[scanCode]; // true if key is pressed and was changed
	}

	bool Input::GetKey(SDL_Scancode scanCode) {
		return pressed[scanCode]; // true if key is pressed
	}

	bool Input::GetKeyUp(SDL_Scancode scanCode) {
		return !pressed[scanCode] && changed[scanCode]; // true if key is not pressed and was changed
	}

	bool Input::GetMouseButtonDown(int button) {
		if(button > 0 && button <= SDL_BUTTON_X2) return mousePressed[button - 1] && mouseChanged[button - 1]; // true if mouse button is pressed and was changed
		return false;
	}

	bool Input::GetMouseButton(int button) {
		if(button > 0 && button <= SDL_BUTTON_X2) return mousePressed[button - 1]; // true if mouse button is pressed
		return false;
	}

	bool Input::GetMouseButtonUp(int button) {
		if(button > 0 && button <= SDL_BUTTON_X2) return !mousePressed[button - 1] && mouseChanged[button - 1]; // true if mouse button is not pressed and was changed
		return false;
	}

	glm::vec2 Input::GetMousePosition() {
		return mousePosition;
	}

	glm::vec2 Input::GetMouseMovement() {
		return glm::vec2(mousePosition.x - oldMousePosition.x, mousePosition.y - oldMousePosition.y);
	}

	void Input::Update() {
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		int mouseX = 0, mouseY = 0;

		Uint32 buttons = SDL_GetGlobalMouseState(&mouseX, &mouseY);
		oldMousePosition = mousePosition;
		mousePosition.x = mouseX, mousePosition.y = mouseY;
		for(int i = 1; i < SDL_Scancode::SDL_NUM_SCANCODES; i++) {
			bool isPressed = keys[(SDL_Scancode)i];
			if(pressed[i] != isPressed) {
				pressed[i] = isPressed;
				changed[i] = true;
			}
			else if(changed[i]) changed[i] = false;
		}
		for(int i = 0; i < 5; i++) {
			bool isPressed;

			if(i == 0) isPressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
			if(i == 1) isPressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
			if(i == 2) isPressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
			if(i == 3) isPressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X1);
			if(i == 4) isPressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X2);
			if(mousePressed[i] != isPressed) {
				mousePressed[i] = isPressed;
				mouseChanged[i] = true;
			}
			else if(mouseChanged[i]) mouseChanged[i] = false;
		}
	}
}
