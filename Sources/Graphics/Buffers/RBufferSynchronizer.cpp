#include "Rpch.h"
#include "RBufferSynchronizer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"


namespace At0::Ray
{
	BufferSynchronizer* BufferSynchronizer::s_Instance = nullptr;

	void BufferSynchronizer::Create() { s_Instance = new BufferSynchronizer(); }
	BufferSynchronizer& BufferSynchronizer::Get() { return *s_Instance; }

	void BufferSynchronizer::Destroy() { delete s_Instance; }

	void BufferSynchronizer::Update(void* data, uint32_t size, uint32_t offset)
	{
		m_UniformBuffer.Update(data, size, offset);
	}

	uint32_t BufferSynchronizer::Emplace(uint32_t size, std::optional<uint32_t> alignment)
	{
		uint32_t minBufferAlignment;

		if (!alignment)
			minBufferAlignment = Graphics::Get()
									 .GetPhysicalDevice()
									 .GetProperties()
									 .limits.minUniformBufferOffsetAlignment;
		else
			minBufferAlignment = *alignment;

		// m_UniformBuffer.Emplace(size, minBufferAlignment, offset);
		auto offset = m_NextUniformBufferOffset;
		m_NextUniformBufferOffset += Buffer::PadSizeToAlignment(size, minBufferAlignment);

		return offset;
	}

	void BufferSynchronizer::Copy(uint32_t srcOffset, uint32_t dstOffset, uint32_t size)
	{
		m_UniformBuffer.CopyRange(srcOffset, dstOffset, size);
	}

	BufferSynchronizer::BufferSynchronizer() {}
}  // namespace At0::Ray
