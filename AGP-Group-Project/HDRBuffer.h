#pragma once
#ifndef B00289996_HDR_BUFFER_H
#define B00289996_HDR_BUFFER_H
#include "FrameBuffer.h"
#include <memory>
namespace B00289996 {
	struct Texture;
	class HDRBuffer : public FrameBuffer {
	public:
		HDRBuffer(const unsigned int & width, const unsigned int & height);
		~HDRBuffer();
	protected:
		virtual void Generate() override;
	};
}
#endif // !B00289996_HDR_BUFFER_H