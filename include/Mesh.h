#pragma once
#ifndef B00289996B00227422_MESH_H
#define B00289996B00227422_MESH_H
#include "OpenGLUtility.h"
#include "Bounds.h"
#include <string>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <GL\glew.h>
#include <memory>

namespace B00289996B00227422 {
	class ShaderProgram;
	
	/// <summary> Vertex Specification </summary>
	struct Vertex {
		glm::vec2 textureCoords;
		glm::vec3 position, normal, tangent, biTangent;
		glm::vec4 colour = glm::vec4(1.0f);
	};

	class Mesh {
		friend class MeshRenderer;
	public:
		/// <summary>Initializes a new instance of the <see cref="Mesh"/> class.</summary>
		Mesh();
		/// <summary>Initializes a new instance of the <see cref="Mesh"/> class.</summary>
		/// <param name="meshName">Name of the mesh.</param>
		/// <param name="verts">Am array of vertex specifications.</param>
		/// <param name="meshIndices">The mesh indices.</param>
		/// <param name="meshID">The mesh ID (this is not a VAO).</param>
		Mesh(const std::string & meshName, const std::vector<Vertex> & verts, const std::vector<unsigned int>& meshIndices, const unsigned int & meshID = 0);
		/// <summary>Finalizes an instance of the <see cref="Mesh"/> class.</summary>
		~Mesh();
		/// <summary>Gets the name.</summary>
		/// <returns>the name</returns>
		const std::string& GetName() const;
		/// <summary>Sets the name.</summary>
		/// <param name="meshName">Name of the mesh.</param>
		void SetName(const std::string & meshName);
		/// <summary>Gets the array of vertex specifications that define this mesh.</summary>
		/// <returns>the array vertex specifications</returns>
		const std::vector<Vertex> & GetVertices() const;
		/// <summary>Adds an array of vertex specifications.</summary>
		/// <param name="verts">the array vertex specifications.</param>
		void AddVertices(const std::vector<Vertex> & verts);
		/// <summary>Sets the array of vertex specifications. (replaces any that already exist)</summary>
		/// <param name="verts">the array vertex specifications.</param>
		void SetVertices(const std::vector<Vertex> & verts);
		/// <summary>Gets the index count.</summary>
		/// <returns>the index count</returns>
		unsigned int GetIndexCount() const;
		/// <summary>Gets the indices.</summary>
		/// <returns>The Indices</returns>
		const std::vector<unsigned int> & GetIndices() const;
		/// <summary>Adds indices.</summary>
		/// <param name="meshIndices">an array of indices.</param>
		void AddIndices(const std::vector<unsigned int> & meshIndices);
		/// <summary>Sets the indices of this mesh. (replaces any that already exist)</summary>
		/// <param name="verts">the array indices.</param>
		void SetIndices(const std::vector<unsigned int> & meshIndices);
		/// <summary>Scales the UVs.</summary>
		/// <param name="scale">The scale.</param>
		void ScaleUVs(const float & scale);
		/// <summary>Gets the VAO.</summary>
		/// <returns>the VAO</returns>
		const GLuint GetVAO() const;
		/// <summary>Builds this instance.</summary>
		void Build();
		/// <summary>Renders this instance.</summary>
		void Render();
		/// <summary>Gets the Axis-Aligned Bounding Box that encoses this mesh.</summary>
		/// <returns>the Axis-Aligned Bounding Box</returns>
		const AABB GetAABB();
		/// <summary>Gets the radius of this mesh.</summary>
		/// <returns>the radius</returns>
		const float GetRadius();
	private:
		/// <summary>Recalculates the mesh Bounds.</summary>
		void RecalculateBounds();
		/// <summary>Updates all of the meshes buffers</summary>
		void Update();
		/// <summary>Updates the position, normal, tangent, biTangent and colour buffers.</summary>
		void UpdateVerts();
		/// <summary>Updates the meshes UV coordinates</summary>
		void UpdateUVs();
		/// <summary>Updates the indices.</summary>
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
#endif //!B00289996B00227422_MESH_H
