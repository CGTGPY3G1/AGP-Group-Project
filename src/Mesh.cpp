#include "Mesh.h"
#include "Shader.h"
#include <glm\geometric.hpp>

namespace B00289996B00227422 {

	Mesh::Mesh() {
	}

	Mesh::Mesh(const std::string & meshName, const std::vector<Vertex> & verts, const std::vector<unsigned int>& meshIndices, const unsigned int & meshID) : name(meshName), vertices(verts), indices(meshIndices), id(meshID), radius(0.0f), dirty(true) {
		Build();
	}

	Mesh::~Mesh() {
		glDeleteBuffers(1, &IBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	const std::string & Mesh::GetName() const {
		return name;
	}

	void Mesh::SetName(const std::string & meshName) {
		name = meshName;
	}

	const std::vector<Vertex>& Mesh::GetVertices() const {
		return vertices;
	}

	void Mesh::AddVertices(const std::vector<Vertex>& verts) {
		for(size_t i = 0; i < verts.size(); i++) vertices.push_back(verts[i]);
		UpdateVerts();
	}

	void Mesh::SetVertices(const std::vector<Vertex>& verts) {
		vertices = verts;
		UpdateVerts();
	}

	unsigned int Mesh::GetIndexCount() const {
		return indices.size();
	}

	const std::vector<unsigned int>& Mesh::GetIndices() const {
		return indices;
	}

	void Mesh::AddIndices(const std::vector<unsigned int>& meshIndices) {
		for(size_t i = 0; i < meshIndices.size(); i++) indices.push_back(meshIndices[i]);
		UpdateIndices();
	}

	void Mesh::SetIndices(const std::vector<unsigned int>& meshIndices) {
		indices = meshIndices;
		UpdateIndices();
	}

	void Mesh::ScaleUVs(const float & scale) {
		for(size_t i = 0; i < vertices.size(); i++) vertices[i].textureCoords *= scale;
		UpdateUVs();
	}

	const GLuint Mesh::GetVAO() const {
		return VAO;
	}

	void Mesh::Build() {
		// Generate VBO and VAOs
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		
		// use a single buffer to store all vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//load the vertex spectification data int the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// define the offsets for each buffer
		glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(VERTEX_POSITION);
		
		glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(VERTEX_NORMAL);
		
		glVertexAttribPointer(VERTEX_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(VERTEX_TANGENT);
		
		glVertexAttribPointer(VERTEX_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));
		glEnableVertexAttribArray(VERTEX_BITANGENT);
		
		glVertexAttribPointer(VERTEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
		glEnableVertexAttribArray(VERTEX_TEXCOORD);
		
		glVertexAttribPointer(VERTEX_COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
		glEnableVertexAttribArray(VERTEX_COLOUR);
		// generate and populate the index buffer
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::RecalculateBounds() {
		float newRadius = 0.0f;
		glm::vec3 min(-0.0f), max(0.0f);
		bool first = true;
		for (std::vector<Vertex>::iterator i = vertices.begin(); i < vertices.end(); ++i) {
			const glm::vec3 pos = (*i).position;
			if (first) {//if this is the first iteration of the loop
				min = pos; max = pos; first = false; // set the min and max values 
				newRadius = glm::length(pos); // and the radius
				continue;// restart the loop
			}
			// if the distance to the vertex is greater than the current radius
			// set it as the current radius
			const float len = glm::length(pos);
			if (len > newRadius) newRadius = len; 
			// if any of the positions components are greater than the minimum/maximum values
			// replace the relevant component with that value
			if (pos.x > max.x) max.x = pos.x;
			else if (pos.x < min.x) min.x = pos.x;
			if (pos.y > max.y) max.y = pos.y;
			else if (pos.y < min.y) min.y = pos.y;
			if (pos.z > max.z) max.z = pos.z;
			else if (pos.z < min.z) min.z = pos.z;
		}
		// updat the bounding box and radius
		aabb.min = min; aabb.max = max;
		radius = newRadius;
		dirty = false;
	}

	void Mesh::Update() {
		UpdateVerts();
		UpdateUVs();
		UpdateIndices();
	}

	void Mesh::UpdateVerts() {
		if (!dirty) dirty = true;
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(VERTEX_POSITION);

		glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(VERTEX_NORMAL);

		glVertexAttribPointer(VERTEX_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(VERTEX_TANGENT);

		glVertexAttribPointer(VERTEX_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));
		glEnableVertexAttribArray(VERTEX_BITANGENT);

		glVertexAttribPointer(VERTEX_COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
		glEnableVertexAttribArray(VERTEX_COLOUR);
	}

	void Mesh::UpdateUVs() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
		glEnableVertexAttribArray(VERTEX_TEXCOORD);
	}

	void Mesh::UpdateIndices() {
		if (!dirty) dirty = true;
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::Render() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	const AABB Mesh::GetAABB() {
		// if geometric changes have been made to the mesh, recalculate it's bounds, else return the aabb
		if (dirty) RecalculateBounds();
		return aabb;
	}
	const float Mesh::GetRadius() {
		// if geometric changes have been made to the mesh, recalculate it's radius, else return the stored radius
		if (dirty) RecalculateBounds();
		return radius;
	}
}
