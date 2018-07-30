#pragma once
#ifndef B00289996B00227422_DEPTH_BUFFER_H
#define B00289996B00227422_DEPTH_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>
namespace B00289996B00227422 {
	struct Texture;
	class DepthBuffer : public AbstractFrameBuffer {
	public:
		/// <summary>Initializes a new instance of the <see cref="DepthBuffer"/> class.</summary>
		/// <param name="width">The buffers width.</param>
		/// <param name="height">The buffers height.</param>
 		DepthBuffer(const unsigned int & width = 3076, const unsigned int & height = 3076);
		/// <summary>Finalizes an instance of the <see cref="DepthBuffer"/> class.</summary>
		~DepthBuffer();
		/// <summary>Gets the texture associated with this buffer.</summary>
		/// <returns>the texture</returns>
		const std::shared_ptr<Texture> GetTexture() const;
	protected:
		void Generate() override;
		std::shared_ptr<Texture> texture;
	};
}
#endif // !B00289996B00227422_DEPTH_BUFFER_H