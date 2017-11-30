#pragma once
#ifndef B00289996B00227422_GAME_H
#define B00289996B00227422_GAME_H
#include <memory>
#include <vector>
#include <glm\mat4x4.hpp>
#include "Lights.h"

namespace B00289996B00227422 {
	class GameObject;
	class ShaderProgram;
	class RenderingSystem;
	class ScriptManagementSystem;
	/// <summary>
	/// Game
	/// </summary>
	class Game {
	public:
		/// <summary> Initializes a new instance of the <see cref="Game"/> class. </summary>
		Game();
		/// <summary> Finalizes an instance of the <see cref="Game"/> class. </summary>
		~Game();
		void Update(const float & deltaTime);
		void FixedUpdate(const float & fixedDeltaTime);
		/// <summary> Renders  the Game. </summary>
		void Render();
		/// <summary>
		/// Determines whether the Game is running.
		/// </summary>
		/// <returns> false if game is running</returns>
		const bool GameOver() const;

		void TogglePointLights();
		void ToggleDirectionalLight();
		void SetRunning(const bool & shouldRun);
		const bool IsPaused() const;
	private:
		std::shared_ptr<GameObject> CreateWall(const glm::vec3 & position, const float & angle = 0.0f);

		bool gameOver = false, running = true;
		std::shared_ptr<ShaderProgram> shader, gBufShader, hudShader;
		std::shared_ptr<RenderingSystem> renderingSystem;
		std::shared_ptr<ScriptManagementSystem> scriptManagementSystem;
		bool pointLightsEnabled = false, directionalLightEnabled = true;
		glm::vec3 lastVelocity;
	};
}
#endif // !B00289996B00227422_GAME_H
