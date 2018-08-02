#include "DebugDraw.h"
#include "FileLoader.h"
#include "Mesh.h"
#include "Camera.h"
#include <GL\glew.h>
#include <glm\gtc\matrix_inverse.hpp>
#include <iostream>
namespace B00289996B00227422 {
	unsigned int DebugDraw::VAO = 0, DebugDraw::positionBuffer = 0, DebugDraw::colourBuffer = 0;

	DebugDraw::DebugDraw() {
		glLineWidth(2);
		shader = FileLoader::GetInstance().LoadShader("Shaders\\basic.vert", "Shaders\\basic.frag");
	}

	void DebugDraw::Init() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_POSITION);

		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), colours.data(), GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)VERTEX_COLOUR, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_COLOUR);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	DebugDraw & DebugDraw::GetInstance() {
		static DebugDraw instance;
		return instance;
	}

	DebugDraw::~DebugDraw() {	
		glDeleteBuffers(1, &colourBuffer);
		glDeleteBuffers(1, &positionBuffer);
		glDeleteVertexArrays(1, &VAO);
	}

	void DebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color) {
		verts.push_back(glm::vec3(from.getX(), from.getY(), from.getZ()));
		verts.push_back(glm::vec3(to.getX(), to.getY(), to.getZ()));
		for (size_t i = 0; i < 2; i++) {
			colours.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		noOfVerts += 2;
	}

	void DebugDraw::DrawLine(const glm::vec3 & from, const glm::vec3 & to, const glm::vec3 & colour) {
		// add the line to the cache
		verts.push_back(from);
		verts.push_back(to);
		for (size_t i = 0; i < 2; i++) {
			colours.push_back(colour);
		}
		noOfVerts += 2;
	}

	void DebugDraw::DrawSphere(const Sphere & sphere, const glm::vec3 & colour) {
		const int points = 32;
		const float tau = glm::pi<float>() * 2.0f;
		const float increment = tau / points;
		const float radius = sphere.radius;

		std::vector<glm::vec3> x, y, z;
		float angle = 0.0f;
		for (int i = 0; i < points; i++) {
			const float a = glm::cos(angle) * radius, b = glm::sin(angle) * radius;
			glm::vec3 vert = glm::vec3(0.0f, a, b);
			x.push_back(sphere.centre + vert);
			vert.x = a, vert.y = 0.0f; vert.z = b;
			y.push_back(sphere.centre + vert);
			vert.x = a, vert.y = b; vert.z = 0.0f;
			z.push_back(sphere.centre + vert);
			angle += increment;
		}
		const int end = points - 1;
		for (int i = 0; i < end; i++) {
			DrawLine(x[i], x[i + 1]);
			DrawLine(y[i], y[i + 1]);
			DrawLine(z[i], z[i + 1]);
		}
		DrawLine(x[end], x[0]);
		DrawLine(y[end], y[0]);
		DrawLine(z[end], z[0]);
	}

	void DebugDraw::DrawOBB(const OBB & obb, const glm::vec3 & colour) {
		const glm::mat3 rotationMatrix = glm::inverseTranspose(glm::mat3(glm::mat4_cast(obb.rotation)));
		const glm::vec3 right = rotationMatrix * glm::vec3(1.0f, 0.0f, 0.0f), forward = rotationMatrix * glm::vec3(0.0f, 0.0f, 1.0f), up = rotationMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
		float width = obb.extents.x * 2.0f, height = obb.extents.y * 2.0f, length = obb.extents.z * 2.0f;

		const glm::vec3 lbb = rotationMatrix * -obb.extents + obb.centre, rbb = lbb + right * width, rtb = rbb + up * height, ltb = rtb + right * -width,
			lbf = lbb + forward * length, rbf = lbf + right * width, rtf = rbf + up * height, ltf = rtf + right * -width;
		DrawLine(lbb, rbb, colour);
		DrawLine(rbb, rtb, colour);
		DrawLine(rtb, ltb, colour);
		DrawLine(ltb, lbb, colour);
		DrawLine(lbb, lbf, colour);
		DrawLine(rbb, rbf, colour);
		DrawLine(rtb, rtf, colour);
		DrawLine(ltb, ltf, colour);
		DrawLine(lbf, rbf, colour);
		DrawLine(rbf, rtf, colour);
		DrawLine(rtf, ltf, colour);
		DrawLine(ltf, lbf, colour);
	}

	void DebugDraw::drawContactPoint(const btVector3 & pointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color) {
		// add the contact to the cache
		GLfloat x = pointOnB.getX() + normalOnB.getX() * distance, y = pointOnB.getY() + normalOnB.getY() * distance, z = pointOnB.getZ() + normalOnB.getZ() * distance;

		// add the normal line to the cache
		verts.push_back(glm::vec3(x, y, z));
		verts.push_back(glm::vec3(x + normalOnB.getX() * 0.05f, y + normalOnB.getY() * 0.05f, z + normalOnB.getZ() * 0.05f));
		verts.push_back(glm::vec3(x, y, z));
		verts.push_back(glm::vec3(x - normalOnB.getX() * 0.05f, y - normalOnB.getY() * 0.05f, z - normalOnB.getZ() * 0.05f));
		for (size_t i = 0; i < 4; i++) {
			colours.push_back(glm::vec3(1.0f));
		}
		noOfVerts += 4;
	}

	void DebugDraw::reportErrorWarning(const char * warningString) {
		std::cout << warningString << std::endl;
	}

	void DebugDraw::setDebugMode(int debugMode) {
		this->debugMode = debugMode;
	}

	int DebugDraw::getDebugMode() const {
		return debugMode;
	}
	void DebugDraw::Render() {
		static GLuint id = 0;
		if (noOfVerts != 0) {
			if (shader) {
				std::shared_ptr<Camera> cam = Camera::GetMainCamera().lock();
				if (cam) {

					shader->Bind();
					// update the shader viewprojection transform using the cameras ViewProjection Transform
					shader->SetUniform("viewprojection", cam->GetViewProjection());
					if (VAO == 0) Init();

					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
					glVertexAttribPointer((GLuint) VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(VERTEX_POSITION);

					glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), colours.data(), GL_STATIC_DRAW);
					glVertexAttribPointer((GLuint) VERTEX_COLOUR, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(VERTEX_COLOUR);
					glDrawArrays(GL_LINES, 0, verts.size());
					glBindVertexArray(0);
				}
			}
		}
		FlushLines();
	}
	void DebugDraw::FlushLines() {
		verts.clear();
		colours.clear();
		noOfVerts = 0;
	}
}
