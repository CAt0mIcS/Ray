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
		enum class Type
		{
			Buffer,
			Sampler
		};

	public:
		virtual ~Uniform();
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		std::string_view GetName() const { return m_Name; }
		uint32_t GetSetNumber() const { return m_DescriptorSet->GetSetNumber(); }

		Uniform& operator=(Uniform&& other) noexcept;
		Uniform(Uniform&& other) noexcept;

		Uniform& operator=(const Uniform& other);
		Uniform(const Uniform& other);

	protected:
		Uniform(std::string_view name) : m_Name(name) {}

	protected:
		Scope<DescriptorSet> m_DescriptorSet;

		std::string m_Name;
	};

}  // namespace At0::Ray
