#pragma once
#ifndef B00289996_MESH_H
#define B00289996_MESH_H
#include "OpenGLUtility.h"
#include "Bounds.h"
#include <string>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <GL\glew.h>
#include <memory>

namespace B00289996 {
	class ShaderProgram;
	
	struct Vertex {
		glm::vec2 textureCoords;
		glm::vec3 position, normal, tangent, biTangent;
		glm::vec4 colour = glm::vec4(1.0f);
	};

	class Mesh {
		friend class MeshRenderer;
	public:
		Mesh();
		Mesh(const std::string & meshName, const std::vector<Vertex> & verts, const std::vector<unsigned int>& meshIndices, const unsigned int & meshID = 0);
		//Mesh(const Mesh & other);
		~Mesh();
		const std::string& GetName() const;
		void SetName(const std::string & meshName);
		const std::vector<Vertex> & GetVertices() const;
		void AddVertices(const std::vector<Vertex> & verts);
		void SetVertices(const std::vector<Vertex> & verts);
		unsigned int GetIndexCount() const;
		const std::vector<unsigned int> & GetIndices() const;
		void AddIndices(const std::vector<unsigned int> & meshIndices);
		void SetIndices(const std::vector<unsigned int> & meshIndices);
		void ScaleUVs(const float & scale);
		const GLuint GetVAO() const;
		void Build();
		void Render();
		const AABB GetAABB();
		const float GetRadius();
	private:
		void RecalculateBounds();
		void Update();
		void UpdateVerts();
		void UpdateUVs();
		void UpdateIndices();
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		GLuint VAO, VBO, IBO;
		unsigned int id;
		float radius;
		AABB aabb;
		bool dirty;
	};
}
#endif //!B00289996_MESH_H
