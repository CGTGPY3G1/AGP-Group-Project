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
		/// <summary>Gets the singleton instance.</summary>
		/// <returns>the instance</returns>
		static FileLoader & FileLoader::GetInstance();
		/// <summary>Finalizes an instance of the <see cref="FileLoader"/> class.</summary>
		~FileLoader();
		/// <summary>Loads a model (or the meshes that make a model).</summary>
		/// <param name="filePath">The models file path.</param>
		/// <param name="singleInstance">should the mesh be instanced.</param>
		/// <returns>an array of meshes, if the model exist, else an empty array</returns>
		std::vector<std::shared_ptr<Mesh>> LoadMeshes(const std::string filePath, const bool & singleInstance = true);
		/// <summary>Loads a texture.</summary>
		/// <param name="filePath">The file path of the texture.</param>
		/// <param name="type">The type.</param>
		/// <returns>a texture, if it exist, else an empty shared pointer</returns>
		std::shared_ptr<Texture> LoadTexture(const std::string filePath, const TextureType & type = TextureType::DIFFUSE);
		/// <summary>Loads a shader.</summary>
		/// <param name="vert">The vertex shader location.</param>
		/// <param name="frag">The fragment shader location.</param>
		/// <param name="geom">The geometry shader location.</param>
		/// <returns>a shader program, if sucessful, else an empty shared pointer</returns>
		std::shared_ptr<ShaderProgram> LoadShader(const std::string & vert, const std::string & frag, const std::string & geom = std::string());
		/// <summary>Loads the shader from a number of strings.</summary>
		/// <param name="vert">The vertex shader .</param>
		/// <param name="frag">The fragment shader.</param>
		/// <param name="geom">The geometry shader.</param>
		/// <returns>a shader program, if sucessful, else an empty shared pointer</returns>
		std::shared_ptr<ShaderProgram> LoadShaderFromString(const std::string & vert, const std::string & frag, const std::string & geom = std::string());
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="FileLoader"/> class from being created.
		/// </summary>
		FileLoader() {}
		std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> meshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders;
	};
}

#endif // !B00289996B00227422_MODEL_LOADER_H
