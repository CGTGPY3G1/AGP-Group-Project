#pragma once
#ifndef B00289996B00227422_MESH_RENDERER_H
#define B00289996B00227422_MESH_RENDERER_H
#include "Component.h"
#include "Bounds.h"
#include <memory>

namespace B00289996B00227422 {
	class ShaderProgram;
	class Material;
	class Mesh;

	struct Texture;

	class MeshRenderer : public Component {
	public:
		/// <summary>Initializes a new instance of the <see cref="MeshRenderer"/> class.</summary>
		/// <param name="g">The game object this component is attached to.</param>
		MeshRenderer(const std::weak_ptr<GameObject> & g);
		/// <summary>Finalizes an instance of the <see cref="MeshRenderer"/> class.</summary>
		~MeshRenderer();
		const ComponentType Type() const override { return COMPONENT_MESH_RENDERER; }
		const std::string GetName() const override { return "MeshRenderer"; }
		/// <summary>Adds a mesh.</summary>
		/// <param name="toAdd">the new mesh.</param>
		void AddMesh(const std::shared_ptr<Mesh> & toAdd);
		/// <summary>Adds an array if meshes.</summary>
		/// <param name="toAdd">the new meshes.</param>
		void AddMeshes(const std::vector<std::shared_ptr<Mesh>> & toAdd);
		/// <summary>Gets the mesh.</summary>
		/// <param name="index">The index of the requested mesh.</param>
		/// <returns>the requested mesh, if it exists, else an empty shared pointer</returns>
		const std::shared_ptr<Mesh> GetMesh(const unsigned int index) const;
		/// <summary>Gets all meshes attached to this.</summary>
		/// <returns>an array of meshes, if there are meshes attached, else an empty array</returns>
		const std::vector<std::shared_ptr<Mesh>> GetMeshes() const;
		/// <summary>Sets this mesh renderers material.</summary>
		/// <param name="newMaterial">The new material.</param>
		void SetMaterial(const std::shared_ptr<Material> & newMaterial);
		/// <summary>Gets the material used by this.</summary>
		/// <returns>the material</returns>
		const std::shared_ptr<Material> GetMaterial() const;
		/// <summary>Renders any meshes attached to this.</summary>
		void Render();
		/// <summary>Gets the Axis-Aligned Bounding Box that encoses this, will be calculated using all attached meshes.</summary>
		/// <returns>the Axis-Aligned Bounding Box</returns>
		const AABB GetAABB();
		/// <summary>Gets the radius of this, will be calculated using all attached meshes.</summary>
		/// <returns>the radius</returns>
		const float GetRadius();
	private:
		/// <summary>Recalculates the Bounds.</summary>
		void RecalculateBounds();
		AABB aabb;
		float radius = 0.0f;
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::shared_ptr<Material> material;
		bool dirty = false;
	};
}

#endif // !B00289996B00227422_MESH_RENDERER_H