#pragma once
#ifndef B00289996B00227422_FRAME_BUFFER_H
#define B00289996B00227422_FRAME_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>
namespace B00289996B00227422 {
	struct Texture;
	class FrameBuffer : public AbstractFrameBuffer {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FrameBuffer"/> class.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="autoGenerate">Should the buffer be generate on instantiation?</param>
		FrameBuffer(const unsigned int & width = 1280, const unsigned int & height = 720, const bool & autoGenerate = true);
		/// <summary>
		/// Finalizes an instance of the <see cref="FrameBuffer"/> class.
		/// </summary>
		~FrameBuffer();
		/// <summary>
		/// Gets the texture used by this FBO.
		/// </summary>
		/// <returns>the texture</returns>
		const std::shared_ptr<Texture> GetTexture() const;
		/// <summary>
		/// Gets the depth buffer used by this FBO.
		/// </summary>
		/// <returns>The Depth Buffer</returns>
		const GLuint GetDepthBuffer() const;
	protected:
		virtual void Generate() override;
		std::shared_ptr<Texture> texture;
		GLuint depthBuffer = 0;
	};
}
#endif // !B00289996B00227422_FRAME_BUFFER_H