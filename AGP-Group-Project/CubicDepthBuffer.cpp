#include "CubicDepthBuffer.h"
#include <iostream>
#include "Texture.h"

namespace B00289996 {
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
		glGenTextures(1, &cubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

		for(unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FBO, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Cubic Depth Buffer Generation Failed!" << std::endl;

		texture = std::make_shared<Texture>();
		texture->type = TextureType::CUBE_MAP;
		texture->id = cubeMapID;

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
