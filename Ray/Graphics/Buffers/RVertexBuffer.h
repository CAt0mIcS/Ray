#pragma once

#include "RBuffer.h"
#include "../Core/RSharedBindable.h"


#include "Ray/Core/RMath.h"
#include <vector>


namespace At0::Ray
{
	class DynamicVertex;

	class RAY_EXPORT VertexBuffer : public Buffer, public SharedBindable
	{
	public:
		VertexBuffer(std::string_view tag, const DynamicVertex& vertices);

		// Dynamic vertex buffer, updatable from CPU
		VertexBuffer(std::string_view tag = "Dynamic", VkDeviceSize allocSize = 0);
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		static std::string GetUID(std::string_view tag, const DynamicVertex& vertices);
		static std::string GetUID(std::string_view tag = "Dynamic", VkDeviceSize allocSize = 0);

		static void SetAllocationSize(VkDeviceSize size) { s_AllocSize = size; }

	private:
		static VkDeviceSize s_AllocSize;
	};
}  // namespace At0::Ray
