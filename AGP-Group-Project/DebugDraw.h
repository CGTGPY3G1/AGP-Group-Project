#pragma once
#ifndef B00289996B00227422_DEBUG_DRAW_H
#define B00289996B00227422_DEBUG_DRAW_H
#include "Bounds.h"
#include <glm\vec3.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <memory>
namespace B00289996B00227422 {

	class ShaderProgram;
	/// <summary>
	/// Debug Drawer
	/// </summary>
	/// <seealso cref="btIDebugDraw" />
	class DebugDraw : public btIDebugDraw {
	public:
		static DebugDraw & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="DebugDraw"/> class.
		/// </summary>
		~DebugDraw();
		/// <summary>
		/// Caches lines. Used by Bullet Physics to draw colliders
		/// </summary>
		/// <param name="from">the start point.</param>
		/// <param name="to">the end point.</param>
		/// <param name="color">The colour (Not Used).</param>
		void drawLine(const btVector3 &from, const btVector3 &to,
			const btVector3 &color);
		void DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec3 &colour = glm::vec3(0.0f, 1.0f, 0.0f));
		void DrawSphere(const Sphere & sphere, const glm::vec3 & colour = glm::vec3(0.0f, 1.0f, 0.0f));
		void DrawOBB(const OBB & obb, const glm::vec3 &colour = glm::vec3(0.0f, 1.0f, 0.0f));

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		/// <summary>
		/// Reports the error warning.
		/// </summary>
		/// <param name="warningString">The warning string.</param>
		void reportErrorWarning(const char *warningString);
		/// <summary>
		/// Draws 3D text (Not Implemented).
		/// </summary>
		/// <param name="location">The location.</param>
		/// <param name="textString">The text string.</param>
		void draw3dText(const btVector3 &location, const char *textString) {}
		/// <summary>
		/// Sets the debug mode.
		/// </summary>
		/// <param name="debugMode">The debug mode.</param>
		void setDebugMode(int debugMode);
		/// <summary>
		/// Gets the debug mode.
		/// </summary>
		/// <returns> the debug mode </returns>
		int getDebugMode() const;
		/// <summary>
		/// Renders the cached data
		/// </summary>
		void Render();
		/// <summary>
		/// Flushes the cached data.
		/// </summary>
		void FlushLines();
	private:
		/// <summary>
		/// Initializes a new instance of the <see cref="DebugDraw"/> class.
		/// </summary>
		DebugDraw();
		void Init();
		static unsigned int VAO, positionBuffer, colourBuffer;
	protected:
		int debugMode;
		unsigned int noOfVerts = 0;
		std::vector<glm::vec3> verts, colours;
		std::shared_ptr<ShaderProgram> shader;
	};
}
#endif // !B00289996B00227422_DEBUG_DRAW_H