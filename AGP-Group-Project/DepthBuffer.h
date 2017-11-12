#pragma once
#ifndef B00289996_DEPTH_BUFFER_H
#define B00289996_DEPTH_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>
namespace B00289996 {
	struct Texture;
	class DepthBuffer : public AbstractFrameBuffer {
	public:
		DepthBuffer(const unsigned int & width = 8192, const unsigned int & height = 8192);
		~DepthBuffer();
		const std::shared_ptr<Texture> GetTexture() const;
	protected:
		void Generate() override;
		std::shared_ptr<Texture> texture;
	};
}
#endif // !B00289996_DEPTH_BUFFER_H