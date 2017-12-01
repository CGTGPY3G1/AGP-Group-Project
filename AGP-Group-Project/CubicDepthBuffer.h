#pragma once
#ifndef B00289996B00227422_CUBIC_DEPTH_BUFFER_H
#define B00289996B00227422_CUBIC_DEPTH_BUFFER_H
#include "AbstractFrameBuffer.h"
#include <memory>

namespace B00289996B00227422 {
	struct Texture;
	/// <summary>
	/// stores 6 depth textures in a cubemap
	/// </summary>
	/// <seealso cref="AbstractFrameBuffer" />
	class CubicDepthBuffer : public AbstractFrameBuffer {
	public:
		/// <summary>Initializes a new instance of the <see cref="CubicDepthBuffer"/> class.</summary>
		/// <param name="width">The width of the buffer.</param>
		/// <param name="height">The height of the buffer.</param>
		CubicDepthBuffer(const unsigned int & width = 1024, const unsigned int & height = 1024);
		/// <summary>Finalizes an instance of the <see cref="CubicDepthBuffer"/> class.</summary>
		~CubicDepthBuffer();
		/// <summary>Gets the texture associated with this buffer.</summary>
		/// <returns>the texture</returns>
		const std::shared_ptr<Texture> GetTexture() const;
	protected:
		void Generate() override;
		std::shared_ptr<Texture> texture;
	};
}
#endif // !B00289996B00227422_CUBIC_DEPTH_BUFFER_H