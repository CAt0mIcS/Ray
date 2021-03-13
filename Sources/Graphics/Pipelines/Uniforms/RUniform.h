#pragma once

#include "../../../RBase.h"

#include "../../Core/RBindable.h"
#include "../RPipeline.h"
#include "RDescriptor.h"
#include "../../Buffers/RBufferSynchronizer.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Uniform : public Bindable
	{
	public:
		virtual ~Uniform();
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		Uniform& operator=(Uniform&& other) noexcept;
		Uniform(Uniform&& other) noexcept;

	protected:
		Uniform() = default;

	protected:
		Scope<DescriptorSet> m_DescriptorSet;
	};

}  // namespace At0::Ray
