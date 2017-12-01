#include "CubicDepthBuffer.h"
#include <iostream>
#include "Texture.h"

namespace B00289996B00227422 {
	CubicDepthBuffer::CubicDepthBuffer(const unsigned int & width, const unsigned int & height) : AbstractFrameBuffer(width, height) {
		Generate();
	}

	CubicDepthBuffer::~CubicDepthBuffer() {
	}

	const std::shared_ptr<Texture> CubicDepthBuffer::GetTexture() const {
		return texture;
	}

	void CubicDepthBuffer::Generate() {
		GLuint cubeMapID;
		// generate a new cubemap
		glGenTextures(1, &cubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
		// create the textures to fill the cubemap
		for(unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		// make sure the UVs are clampedto the edge, ignoring the border
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// create an FBO and add a depth attachment
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FBO, 0);
		// do not read or write to a colour buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		// print an error to the console if the cubic depth buffer is incomplete
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Cubic Depth Buffer Generation Failed!" << std::endl;
		// create a new Texture struct to store the textures data
		texture = std::make_shared<Texture>();
		texture->type = TextureType::CUBE_MAP;
		texture->id = cubeMapID;

		// bind the default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
