#pragma once
#ifndef B00289996_MESH_RENDERER_H
#define B00289996_MESH_RENDERER_H
#include "Component.h"
#include "Bounds.h"
#include <memory>

namespace B00289996 {
	class ShaderProgram;
	class Material;
	class Mesh;

	struct Texture;

	class MeshRenderer : public Component {
	public:
		MeshRenderer(const std::weak_ptr<GameObject> & g);
		~MeshRenderer();
		const ComponentType Type() const override { return COMPONENT_MESH_RENDERER; }
		const std::string GetName() const override { return "MeshRenderer"; }
		void AddMesh(const std::shared_ptr<Mesh> & toAdd);
		void AddMeshes(const std::vector<std::shared_ptr<Mesh>> & toAdd);
		const std::shared_ptr<Mesh> GetMesh(const unsigned int index) const;
		const std::vector<std::shared_ptr<Mesh>> GetMeshes() const;
		void SetMaterial(const std::shared_ptr<Material> & newMaterial);
		const std::shared_ptr<Material> GetMaterial() const;
		void Render();
		const AABB GetAABB();
		const float GetRadius();
	private:
		void RecalculateBounds();
		AABB aabb;
		float radius = 0.0f;
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::shared_ptr<Material> material;
	};
}

#endif // !B00289996_MESH_RENDERER_H