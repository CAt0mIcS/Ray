#pragma once

#include "../../RBase.h"
#include "RBuffer.h"
#include "../Core/RSharedBindable.h"

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>


namespace At0::Ray
{
	class CommandBuffer;

	class RAY_EXPORT IndexBuffer : public Buffer, public SharedBindable
	{
	public:
		using Type = uint16_t;

	public:
		IndexBuffer(std::string_view tag, const std::vector<IndexBuffer::Type>& indices);
		~IndexBuffer() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;
		uint32_t GetNumberOfIndices() const;

		static std::string GetUID(
			std::string_view tag, const std::vector<IndexBuffer::Type>& indices);

	private:
		uint32_t m_NumIndices = 0;
	};
}  // namespace At0::Ray
