#pragma once
#ifndef B00289996B00227422_DEPTH_BUFFER_H
#define B00289996B00227422_DEPTH_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>
namespace B00289996B00227422 {
	struct Texture;
	class DepthBuffer : public AbstractFrameBuffer {
	public:
		DepthBuffer(const unsigned int & width = 3076, const unsigned int & height = 3076);
		~DepthBuffer();
		const std::shared_ptr<Texture> GetTexture() const;
	protected:
		void Generate() override;
		std::shared_ptr<Texture> texture;
	};
}
#endif // !B00289996B00227422_DEPTH_BUFFER_H