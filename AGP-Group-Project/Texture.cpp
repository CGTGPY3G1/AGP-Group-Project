#include "Texture.h"

namespace B00289996 {
	Texture::~Texture() {
		glDeleteTextures(1, &id);
	}
	void Texture::Bind(std::shared_ptr<ShaderProgram> shader) {
		if(type == TextureType::DIFFUSE) {
			static GLuint boundTexture = 0;
			glActiveTexture(GL_TEXTURE0);
			if(id != boundTexture) {
				boundTexture = id;
				glBindTexture(GL_TEXTURE_2D, id);
				shader->SetUniform("textureUnit0", 0);
			}
		}
		else if(type == TextureType::NORMAL_MAP) {
			static GLuint boundTexture = 0;
			glActiveTexture(GL_TEXTURE1);
			if(id != boundTexture) {
				boundTexture = id;
				glBindTexture(GL_TEXTURE_2D, id);
				shader->SetUniform("textureUnit1", 1);
			}
		}
		else if(type == TextureType::DEPTH_MAP) {
			static GLuint boundTexture = 0;
			glActiveTexture(GL_TEXTURE2);
			if(id != boundTexture) {
				boundTexture = id;
				glBindTexture(GL_TEXTURE_2D, id);
				shader->SetUniform("textureUnit2", 2);
			}
		}
		else if(type == TextureType::HEIGHT_MAP) {
			static GLuint boundTexture = 0;
			glActiveTexture(GL_TEXTURE3);
			if(id != boundTexture) {
				boundTexture = id;
				glBindTexture(GL_TEXTURE_2D, id);
				shader->SetUniform("textureUnit3", 3);
			}
		}
		else if(type == TextureType::CUBE_MAP) {
			static GLuint boundTexture = 0;
			glActiveTexture(GL_TEXTURE4);
			if(id != boundTexture) {
				boundTexture = id;
				glBindTexture(GL_TEXTURE_CUBE_MAP, id);
				shader->SetUniform("cubeTexture", 4);
			}
		}
	}

	void Texture::UnBind() {
		glActiveTexture(GL_TEXTURE0 + (unsigned int)type);
		if(type == TextureType::CUBE_MAP) glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		else glBindTexture(GL_TEXTURE_2D, 0);
	}
}
