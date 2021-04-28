#include "Rpch.h"
#include "RUniform.h"

#include "Graphics/RGraphics.h"
#include "Utils/RString.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"


namespace At0::Ray
{
	Uniform::~Uniform() {}

	void Uniform::CmdBind(const CommandBuffer& cmdBuff) const { m_DescriptorSet->CmdBind(cmdBuff); }

	Uniform& Uniform::operator=(Uniform&& other) noexcept
	{
		if (other.m_DescriptorSet)
			m_DescriptorSet = std::move(other.m_DescriptorSet);
		return *this;
	}

	Uniform::Uniform(Uniform&& other) noexcept { *this = std::move(other); }

	Uniform& Uniform::operator=(const Uniform& other)
	{
		m_Name = other.m_Name;
		m_DescriptorSet = MakeScope<DescriptorSet>(*other.m_DescriptorSet);
		return *this;
	}

	Uniform::Uniform(const Uniform& other) { *this = other; }
}  // namespace At0::Ray
