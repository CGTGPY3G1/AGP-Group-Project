#pragma once
#ifndef B00289996B00227422_TEXTURE_H
#define B00289996B00227422_TEXTURE_H
#include <string>
#include <GL\glew.h>
#include <memory>
#include "Shader.h"
namespace B00289996B00227422 {
	/// <summary>Texture Types</summary>
	enum TextureType : unsigned int {
		DIFFUSE = 0,
		NORMAL_MAP = 1,
		DEPTH_MAP = 2,
		HEIGHT_MAP = 3,
		CUBE_MAP = 4
	};

	struct Texture : std::enable_shared_from_this<Texture> {
		TextureType type;
		GLuint id;
		~Texture();
		unsigned int width = 1, height = 1;
		/// <summary>Binds the texture to the provided shader</summary>
		/// <param name="shader">The shader.</param>
		void Bind(std::shared_ptr<ShaderProgram> shader);
		/// <summary>Unbind the texture</summary>
		void UnBind();
	};

	/// <summary>Used to find texture of a certain type in a collection</summary>
	struct TextureIsType {
		TextureIsType(const TextureType & type) : t(type) {}
		bool operator()(std::shared_ptr<Texture> l) { return l->type == t; }
	private:
		TextureType t;
	};
}
#endif // !B00289996B00227422_TEXTURE_H
