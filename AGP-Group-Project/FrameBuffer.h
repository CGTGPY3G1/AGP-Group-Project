#pragma once
#ifndef B00289996B00227422_FRAME_BUFFER_H
#define B00289996B00227422_FRAME_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>
namespace B00289996B00227422 {
	struct Texture;
	class FrameBuffer : public AbstractFrameBuffer {
	public:
		FrameBuffer(const unsigned int & width = 1280, const unsigned int & height = 720, const bool & autoGenerate = true);
		~FrameBuffer();
		const std::shared_ptr<Texture> GetTexture() const;
		const GLuint GetDepthBuffer() const;
	protected:
		virtual void Generate() override;
		std::shared_ptr<Texture> texture;
		GLuint depthBuffer = 0;
	};
}
#endif // !B00289996B00227422_FRAME_BUFFER_H