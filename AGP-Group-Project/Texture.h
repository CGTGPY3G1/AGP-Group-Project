#pragma once
#ifndef B00289996_TEXTURE_H
#define B00289996_TEXTURE_H
#include <string>
#include <GL\glew.h>
#include <memory>
#include "Shader.h"
namespace B00289996 {
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
		unsigned int width = 1, height = 1;
		void Bind(std::shared_ptr<ShaderProgram> shader);
		void UnBind();
	};

	struct TextureIsType {
		TextureIsType(const TextureType & type) : t(type) {}
		bool operator()(std::shared_ptr<Texture> l) { return l->type == t; }
	private:
		TextureType t;
	};
}
#endif // !B00289996_TEXTURE_H
