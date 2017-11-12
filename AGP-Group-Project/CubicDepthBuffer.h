#pragma once
#ifndef B00289996_CUBIC_DEPTH_BUFFER_H
#define B00289996_CUBIC_DEPTH_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>

namespace B00289996 {
	struct Texture;
	class CubicDepthBuffer : public AbstractFrameBuffer {
	public:
		CubicDepthBuffer(const unsigned int & width = 1024, const unsigned int & height = 1024);
		~CubicDepthBuffer();
		const std::shared_ptr<Texture> GetTexture() const;
	protected:
		void Generate() override;
		std::shared_ptr<Texture> texture;
	};
}
#endif // !B00289996_CUBIC_DEPTH_BUFFER_H