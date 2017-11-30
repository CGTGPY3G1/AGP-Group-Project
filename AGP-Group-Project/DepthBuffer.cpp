#include "DepthBuffer.h"
#include <iostream>
#include "Texture.h"
namespace B00289996B00227422 {
	DepthBuffer::DepthBuffer(const unsigned int & width, const unsigned int & height) : AbstractFrameBuffer(width, height) {
		Generate();
	}

	DepthBuffer::~DepthBuffer() {
	}

	const std::shared_ptr<Texture> DepthBuffer::GetTexture() const {
		return texture;
	}

	void DepthBuffer::Generate() {
		GLuint textureId = 0;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		float borderColor [] = {1.0f, 1.0f, 1.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D, 0);
		GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

		glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Depth Buffer Generation Failed!" << std::endl;
		FBO = fbo;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		texture = std::make_shared<Texture>();
		texture->type = TextureType::DEPTH_MAP;
		texture->id = textureId;
	}
}
