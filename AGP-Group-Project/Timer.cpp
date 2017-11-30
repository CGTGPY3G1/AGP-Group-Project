#include "Timer.h"
#include <SDL_timer.h>
namespace B00289996B00227422 {
	const float Timer::FIXED_DELTA_TIME = 1.0f/60.0f;
	float Timer::deltaTime = 0.0f;
	unsigned int  Timer::currentTime = 0, Timer::lastFrameTime = 0;

	Timer::~Timer() {
	}

	const float Timer::GetDeltaTime() {
		return deltaTime;
	}

	const float Timer::GetFixedDeltaTime() {
		return FIXED_DELTA_TIME;
	}

	const float Timer::GetTime() {
		return SDL_GetTicks() / 1000.0f;
	}

	void Timer::Update() {
		currentTime = SDL_GetTicks();
		deltaTime = ((float)(currentTime - lastFrameTime)) / 1000.0f;
		lastFrameTime = currentTime;
	}

	void Timer::Init() {
		lastFrameTime = SDL_GetTicks();
		currentTime = lastFrameTime;
	}
}