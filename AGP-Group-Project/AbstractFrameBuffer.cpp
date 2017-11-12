#include "AbstractFrameBuffer.h"
namespace B00289996 {
	AbstractFrameBuffer::AbstractFrameBuffer(const unsigned int & width, const unsigned int & height) : w(width), h(height){
		
	}

	AbstractFrameBuffer::~AbstractFrameBuffer() {
	}

	const void AbstractFrameBuffer::Bind() const {
		glViewport(0, 0, w, h);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	const void AbstractFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const GLuint AbstractFrameBuffer::GetFBO() const {
		return FBO;
	}

	const unsigned int AbstractFrameBuffer::GetWidth() const {
		return w;
	}

	const unsigned int AbstractFrameBuffer::GetHeight() const {
		return h;
	}
}
