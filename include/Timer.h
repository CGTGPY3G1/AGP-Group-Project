#pragma once
#ifndef B00289996B00227422_TIMER_H
#define B00289996B00227422_TIMER_H
namespace B00289996B00227422 {
	class Timer {
		friend class Engine;
	public:
		~Timer();
		/// <summary>
		/// Gets the frame time.
		/// </summary>
		/// <returns>the frame time</returns>
		static const float GetDeltaTime();
		/// <summary>
		/// Gets the fixed time step.
		/// </summary>
		/// <returns>the fixed time step</returns>
		static const float GetFixedDeltaTime();
		/// <summary>Gets the total running time.</summary>
		/// <returns>the total run time</returns>
		static const float GetTime();
	private:
		/// <summary>Updates the timer.</summary>
		static void Update();
		/// <summary>Initializes the timer.</summary>
		static void Init();
		static const float FIXED_DELTA_TIME;
		static float deltaTime;
		static unsigned int currentTime, lastFrameTime;
	};
}
#endif // !B00289996B00227422_TIMER_H
