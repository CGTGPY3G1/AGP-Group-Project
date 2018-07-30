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
		// generate a new texture
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// make sure the UVs are clampedto the edge, ignoring the border
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// attach a depth component
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		// create an FBO and add a depth attachment
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
		// do not write to a colour buffer
		glDrawBuffer(GL_NONE);
		// print an error to the console if the depth buffer is incomplete
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Depth Buffer Generation Failed!" << std::endl;
		// create a new Texture struct to store the textures data
		texture = std::make_shared<Texture>();
		texture->type = TextureType::DEPTH_MAP;
		texture->id = textureId;
		// bind the default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
