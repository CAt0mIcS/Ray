#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"
#include "../RPipeline.h"

#include <RayRenderer/Pipeline/RDescriptor.h>


namespace At0::Ray
{
	class BufferUniform;
	class Sampler2DUniform;

	class RAY_EXPORT DescriptorSet : NonCopyable
	{
	public:
		DescriptorSet(RrDescriptorPool pool, RrDescriptorSetLayout descriptorLayout,
			Pipeline::BindPoint pipelineBindPoint, RrPipelineLayout pipelineLayout,
			uint32_t setNumber);

		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const;
		static void Update(const std::vector<RrWriteDescriptorSet>& descriptorWrites);
		uint32_t GetSetNumber() const { return m_SetNumber; }

		void BindUniform(const BufferUniform& uniform);
		void BindUniform(const Sampler2DUniform& uniform);

		operator RrDescriptorSet() const { return m_DescriptorSet; }
		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept;

	private:
		RrDescriptorSet m_DescriptorSet = nullptr;
		RrDescriptorPool m_DescriptorPool = nullptr;
		RrDescriptorSetLayout m_DescriptorSetLayout = nullptr;
		uint32_t m_SetNumber;

		Pipeline::BindPoint m_PipelineBindPoint;
		RrPipelineLayout m_PipelineLayout = nullptr;

#ifndef NDEBUG
		bool m_UniformBound = false;
#endif
	};
}  // namespace At0::Ray
