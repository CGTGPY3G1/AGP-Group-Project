#include "Material.h"
#include "Shader.h"

#include <glm\common.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <algorithm>

namespace B00289996B00227422 {
	const float Material::min = 0.0f, Material::max = 1.0f;

	void Material::Bind(const std::shared_ptr<ShaderProgram> & differentShader) {
		// if an alternative shader has been provided, use it, else, use the materials shader shader
		std::shared_ptr<ShaderProgram> currentShader = (differentShader.use_count() > 0) ? differentShader : shader;
		currentShader->Bind();
		// set the material uniforms
		currentShader->SetUniform<glm::vec4>("material.ambient", ambient);
		currentShader->SetUniform<glm::vec4>("material.diffuse", diffuse);
		currentShader->SetUniform<glm::vec4>("material.specular", specular);
		currentShader->SetUniform<float>("material.shininess", shininess);
		bool useNormalMap = false;
		for(std::vector<std::shared_ptr<Texture>>::iterator i = textures.begin(); i != textures.end(); ++i) {
			// bind the texture to the shader
			(*i)->Bind(currentShader);
			// if a normal map is found, enable normal mapping in the shader
			if(!useNormalMap && (*i)->type == NORMAL_MAP) useNormalMap = true; 
		}
		currentShader->SetUniform<bool>("useNormalMap", useNormalMap);
	}

	Material::Material() : ambient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)), diffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), specular(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)), shininess(0.8f) {
	}

	Material::~Material() {
	}

	const std::shared_ptr<ShaderProgram> Material::GetShader() const {
		return shader;
	}

	void Material::SetShader(const std::shared_ptr<ShaderProgram>& shader) {
		this->shader = shader;
	}

	void Material::SetTexture(const std::shared_ptr<Texture>& toAdd) {
		if(textures.size() > 0) textures.clear();
		textures.push_back(toAdd);
	}

	void Material::ReplaceTexture(const unsigned int & index, const std::shared_ptr<Texture> & newTexture, const bool & addIfNotFound) {
		if(index < textures.size()) textures[index] = newTexture;
		else if(addIfNotFound) textures.push_back(newTexture);
	}

	void Material::ReplaceTexture(const TextureType & type, const std::shared_ptr<Texture> & newTexture, const bool & addIfNotFound) {
		bool found = false;
		for(size_t i = 0; i < textures.size(); i++) {
			if(textures[i]->type == type) {
				found = true;
				textures[i] = newTexture;
				break;
			}
		}
		if(!found && addIfNotFound) textures.push_back(newTexture);
	}

	void Material::RemoveTexture(const unsigned int & index) {
		if(index < textures.size()) textures.erase(textures.begin() + index);
	}

	void Material::RemoveTextureType(const TextureType & type) {
		std::vector<std::shared_ptr<Texture>>::iterator it = std::find_if(textures.begin(), textures.end(), TextureIsType(type));
		while(it != textures.end()) {
			textures.erase(it);
			it = std::find_if(textures.begin(), textures.end(), TextureIsType(type));
		}
	}

	void Material::AddTexture(const std::shared_ptr<Texture>& toAdd) {
		if(toAdd) textures.push_back(toAdd);
	}

	void Material::AddTextures(const std::vector<std::shared_ptr<Texture>>& toAdd) {
		for(size_t i = 0; i < toAdd.size(); i++) {
			if(toAdd[i]) textures.push_back(toAdd[i]);
		}
	}

	const std::shared_ptr<Texture> Material::GetTexture(const unsigned int & index) const {
		return index < textures.size() ? textures[index] : std::shared_ptr<Texture>();
	}

	const std::vector<std::shared_ptr<Texture>>& Material::GetTextures() const {
		return textures;
	}

	const glm::vec4 Material::GetAmbient() const {
		return ambient;
	}

	void Material::SetAmbient(const glm::vec4 & newValue) {
		ambient = glm::clamp(newValue, min, max);
	}

	const glm::vec4 Material::GetDiffuse() const {
		return diffuse;
	}

	void Material::SetDiffuse(const glm::vec4 & newValue) {
		diffuse = glm::clamp(newValue, min, max);
	}

	const glm::vec4 Material::GetSpecular() const {
		return specular;
	}

	void Material::SetSpecular(const glm::vec4 & newValue) {
		specular = glm::clamp(newValue, min, max);
	}

	const float Material::GetShininess() const {
		return shininess;
	}

	void Material::SetShininess(const float & newValue) {
		shininess = glm::clamp(newValue, min, max);
	}
}
