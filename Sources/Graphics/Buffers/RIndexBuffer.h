#pragma once

#include "../../RBase.h"
#include "RBuffer.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class RAY_EXPORT IndexBuffer : public Buffer
	{
	public:
		using Type = uint16_t;

	public:
		IndexBuffer(const std::vector<IndexBuffer::Type>& indices);
		~IndexBuffer() = default;

		uint32_t GetNumberOfIndices() const;

	private:
		uint32_t m_NumIndices = 0;
	};
}  // namespace At0::Ray
