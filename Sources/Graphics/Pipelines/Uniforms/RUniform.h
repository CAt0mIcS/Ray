#pragma once

#include "../../../RBase.h"

#include "../../Core/RBindable.h"
#include "../RPipeline.h"
#include "RDescriptor.h"
#include "../../Buffers/RBufferSynchronizer.h"

#include <vulkan/vulkan_core.h>
#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Uniform : public Bindable
	{
	public:
		virtual ~Uniform();
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		std::string_view GetName() const { return m_Name; }

		Uniform& operator=(Uniform&& other) noexcept;
		Uniform(Uniform&& other) noexcept;

	protected:
		Uniform(std::string_view name) : m_Name(name){};

	protected:
		Scope<DescriptorSet> m_DescriptorSet;
		std::string m_Name;
	};

}  // namespace At0::Ray
