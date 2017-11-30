#pragma once
#ifndef B00289996B00227422_MODEL_LOADER_H
#define B00289996B00227422_MODEL_LOADER_H
#include <unordered_map>
#include <vector>
#include "Texture.h"

struct aiScene;
namespace B00289996B00227422 {
	class ShaderProgram;
	class Mesh;
	
	class FileLoader {
	public:
		static FileLoader & FileLoader::GetInstance();
		~FileLoader();
		std::vector<std::shared_ptr<Mesh>> LoadMeshes(const std::string filePath, const bool & singleInstance = true);		
		std::shared_ptr<Texture> LoadTexture(const std::string filePath, const TextureType & type = TextureType::DIFFUSE);
		std::shared_ptr<ShaderProgram> LoadShader(const std::string & vert, const std::string & frag, const std::string & geom = std::string());
		std::shared_ptr<ShaderProgram> LoadShaderFromString(const std::string & vert, const std::string & frag, const std::string & geom = std::string());
	private:
		FileLoader() {}
		std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> meshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders;
	};
}

#endif // !B00289996B00227422_MODEL_LOADER_H
