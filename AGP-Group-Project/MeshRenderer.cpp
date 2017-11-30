#include "MeshRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Mesh.h"
namespace B00289996B00227422 {
	MeshRenderer::MeshRenderer(const std::weak_ptr<GameObject>& g) : Component(g) {
	}

	MeshRenderer::~MeshRenderer() {
	}

	void MeshRenderer::AddMesh(const std::shared_ptr<Mesh> & toAdd) {
		meshes.push_back(toAdd);
		RecalculateBounds();
	}

	void MeshRenderer::AddMeshes(const std::vector<std::shared_ptr<Mesh>> & toAdd) {
		for(size_t i = 0; i < toAdd.size(); i++) {
			meshes.push_back(toAdd[i]);
		}
		RecalculateBounds();
	}

	const std::shared_ptr<Mesh> MeshRenderer::GetMesh(const unsigned int index) const {
		if(index >= 0 && index < meshes.size()) return meshes[index];
		return std::shared_ptr<Mesh>();
	}

	const std::vector<std::shared_ptr<Mesh>> MeshRenderer::GetMeshes() const {
		return meshes;
	}

	void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& newMaterial) {
		material = newMaterial;
	}

	const std::shared_ptr<Material> MeshRenderer::GetMaterial() const {
		return material;
	}

	void MeshRenderer::Render() {
		material->Bind();
		std::shared_ptr<ShaderProgram> shader = material->GetShader();
		std::shared_ptr<Transform> transform = GetComponent<Transform>().lock();
		shader->SetUniform("modelview", transform->GetWorldTransform());
		shader->SetUniform("normalMatrix", transform->GetNormalMatrix());
		for(std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); ++i) {
			(*i)->Render();
		}
	}

	const AABB MeshRenderer::GetAABB() {
		return aabb;
	}

	const float MeshRenderer::GetRadius() {
		return radius;
	}

	void MeshRenderer::RecalculateBounds() {
		glm::vec3 min(0.0f), max(0.0f);
		float rad = 0.0f;
		for (std::vector<std::shared_ptr<Mesh>>::iterator i = meshes.begin(); i != meshes.end(); i++) {
			const AABB mAABB = (*i)->GetAABB();
			if (mAABB.min.x < min.x) min.x = mAABB.min.x;
			if (mAABB.max.x > max.x) max.x = mAABB.max.x;
			if (mAABB.min.y < min.y) min.y = mAABB.min.y;
			if (mAABB.max.y > max.y) max.y = mAABB.max.y;
			if (mAABB.min.z < min.z) min.z = mAABB.min.z;
			if (mAABB.max.z > max.z) max.z = mAABB.max.z;
			if ((*i)->GetRadius() > rad) rad = (*i)->GetRadius();
		}
		aabb.min = min; aabb.max = max;
		radius = rad;
	}
}
