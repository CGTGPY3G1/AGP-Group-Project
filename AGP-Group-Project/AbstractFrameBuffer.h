#pragma once
#ifndef B00289996_ABSTRACT_FRAME_BUFFER_H
#define B00289996_ABSTRACT_FRAME_BUFFER_H
#include <GL/glew.h>
namespace B00289996 {
	class AbstractFrameBuffer{
	public:
		AbstractFrameBuffer(const unsigned int & width, const unsigned int & height);
		~AbstractFrameBuffer();
		virtual const void Bind() const;
		virtual const void Unbind() const;
		const GLuint GetFBO() const;
		const unsigned int GetWidth() const;
		const unsigned int GetHeight() const;
	protected:
		virtual void Generate() = 0;
		unsigned int w, h;
		GLuint FBO = 0;
	};
}
#endif // !B00289996_ABSTRACT_FRAME_BUFFER_H