#pragma once
#ifndef B00289996B00227422_B00227422_ABSTRACT_FRAME_BUFFER_H
#define B00289996B00227422_B00227422_ABSTRACT_FRAME_BUFFER_H
#include <GL/glew.h>
namespace B00289996B00227422 {
	/// <summary>
	/// Used to represent multiple OpenGL Frame Buffer types
	/// </summary>
	class AbstractFrameBuffer {
	public:
		/// <summary> Initializes a new instance of the <see cref="AbstractFrameBuffer"/> class. </summary>
		/// <param name="width">The width of the frame buffer.</param>
		/// <param name="height">The height of the frame buffer.</param>
		AbstractFrameBuffer(const unsigned int & width, const unsigned int & height);
		/// <summary> Finalizes an instance of the <see cref="AbstractFrameBuffer"/> class. </summary>
		~AbstractFrameBuffer();
		/// <summary> Binds the frame buffer. </summary>
		virtual void Bind() const;
		/// <summary> Unbinds the frame buffer. </summary>
		virtual void Unbind() const;
		/// <summary> Gets the fbo. </summary>
		/// <returns>the fbo</returns>
		const GLuint GetFBO() const;
		/// <summary> Gets the frame buffers width. </summary>
		/// <returns>the frame buffers width</returns>
		const unsigned int GetWidth() const;
		/// <summary> Gets the frame buffers height. </summary>
		/// <returns>the frame buffers width</returns>
		const unsigned int GetHeight() const;
	protected:
		/// <summary> Generates the frame buffers width. </summary>
		virtual void Generate() = 0;
		// the width and height
		unsigned int w, h;
		GLuint FBO = 0;
	};
}
#endif // !B00289996B00227422_ABSTRACT_FRAME_BUFFER_H