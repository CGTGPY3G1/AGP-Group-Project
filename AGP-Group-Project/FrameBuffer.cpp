#include "FrameBuffer.h"
#include "Texture.h"
#include <iostream>

namespace B00289996 {

	FrameBuffer::FrameBuffer(const unsigned int & width, const unsigned int & height, const bool & autoGenerate) : AbstractFrameBuffer(width, height) {
		if(autoGenerate) Generate();
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &FBO);
	}

	const std::shared_ptr<Texture> FrameBuffer::GetTexture() const {
		return texture;
	}

	const GLuint FrameBuffer::GetDepthBuffer() const {
		return depthBuffer;
	}

	void FrameBuffer::Generate() {
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		texture = std::make_shared<Texture>();
		texture->type = TextureType::DIFFUSE;
		texture->width = w;
		texture->height = h;
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		// create an empty texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// create a depth buffer
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		// attach the texture with a colour attachment
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->id, 0);
		GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);
		
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame Buffer Generation Failed!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind the buffer
	}
}
