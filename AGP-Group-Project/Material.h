#pragma once
#ifndef B00289996_MATERIAL_H
#define B00289996_MATERIAL_H
#include "Texture.h"
#include "Mesh.h"
#include <memory>
namespace B00289996 {

	struct OpenGLMaterial {
		
	};
	class ShaderProgram;
	struct Texture;
	class Material {
	public:
		Material();
		~Material();
		const std::shared_ptr<ShaderProgram> GetShader() const;
		void SetShader(const std::shared_ptr<ShaderProgram> & shader);
		void SetTexture(const std::shared_ptr<Texture> & toAdd);
		void ReplaceTexture(const unsigned int & index, const std::shared_ptr<Texture> & newTexture, const bool & addIfNotFound = true);
		void ReplaceTexture(const TextureType & type, const std::shared_ptr<Texture> & newTexture, const bool & addIfNotFound = true);
		void RemoveTexture(const unsigned int & index);
		void RemoveTextureType(const TextureType & type);
		void AddTexture(const std::shared_ptr<Texture> & toAdd);
		void AddTextures(const std::vector<std::shared_ptr<Texture>> & toAdd);
		const std::shared_ptr<Texture> & GetTexture(const unsigned int & index) const;
		const std::vector<std::shared_ptr<Texture>> & GetTextures() const;
		const glm::vec4 GetAmbient() const;
		void SetAmbient(const glm::vec4 & newValue);
		const glm::vec4 GetDiffuse() const;
		void SetDiffuse(const glm::vec4 & newValue);
		const glm::vec4 GetSpecular() const;
		void SetSpecular(const glm::vec4 & newValue);
		const float GetShininess() const;
		void SetShininess(const float & newValue);
		void Bind(const std::shared_ptr<ShaderProgram> & differentShader = std::shared_ptr<ShaderProgram>());
	private:
		glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), specular = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
		float shininess = 0.8f;
		std::shared_ptr<ShaderProgram> shader;
		std::vector<std::shared_ptr<Texture>> textures;
		const static float min, max;
	};
}

#endif // !B00289996_MATERIAL_H
