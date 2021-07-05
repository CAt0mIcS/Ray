#include "Rpch.h"
#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Core/RRendererLoader.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "../RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RTexture2D.h"
#include "RBufferUniform.h"
#include "RSampler2DUniform.h"

#include "Ray/Utils/RException.h"
#include "Ray/Utils/RLogger.h"


namespace At0::Ray
{
	DescriptorSet::DescriptorSet(RrDescriptorPool pool, RrDescriptorSetLayout descriptorLayout,
		Pipeline::BindPoint pipelineBindPoint, RrPipelineLayout pipelineLayout, uint32_t setNumber)
		: m_PipelineBindPoint(pipelineBindPoint), m_PipelineLayout(pipelineLayout),
		  m_SetNumber(setNumber), m_DescriptorPool(pool), m_DescriptorSetLayout(descriptorLayout)
	{
		RrDescriptorSetAllocateInfo allocInfo{};
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;

		ThrowRenderError(RendererAPI::AllocateDescriptorSets(
							 Graphics::Get().GetDevice(), &allocInfo, &m_DescriptorSet),
			"[DescriptorSet] Failed to allocate");
	}

	void DescriptorSet::CmdBind(const CommandBuffer& cmdBuff) const
	{
#ifndef NDEBUG
		if (!m_UniformBound)
			Log::Warn("[DescriptorSet] No uniforms bound to descriptor set {0}", m_SetNumber);
#endif
		RendererAPI::CmdBindDescriptorSets(cmdBuff, (RrPipelineBindPoint)m_PipelineBindPoint,
			m_PipelineLayout, m_SetNumber, 1, &m_DescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Update(const std::vector<RrWriteDescriptorSet>& descriptorWrites)
	{
		RendererAPI::UpdateDescriptorSets(Graphics::Get().GetDevice(),
			(uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::BindUniform(const BufferUniform& uniform)
	{
		RrDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniform.GetUniformBuffer().GetBuffer();
		bufferInfo.offset = uniform.GetOffset();
		bufferInfo.range = uniform.GetSize();

		RrWriteDescriptorSet descWrite{};
		descWrite.dstSet = m_DescriptorSet;
		descWrite.dstBinding = uniform.GetBinding();
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = RrDescriptorTypeUniformBuffer;
		descWrite.pBufferInfo = &bufferInfo;

		Update({ descWrite });

#ifndef NDEBUG
		m_UniformBound = true;
#endif
	}

	void DescriptorSet::BindUniform(const Sampler2DUniform& uniform)
	{
		RrDescriptorImageInfo imageInfo{};
		imageInfo.sampler = uniform.GetTexture()->GetSampler();
		imageInfo.imageView = uniform.GetTexture()->GetImageView();
		imageInfo.imageLayout = uniform.GetTexture()->GetImageLayout();

		RrWriteDescriptorSet descWrites{};
		descWrites.dstSet = m_DescriptorSet;
		descWrites.dstBinding = uniform.GetBinding();
		descWrites.dstArrayElement = 0;
		descWrites.descriptorCount = 1;
		descWrites.descriptorType = RrDescriptorTypeCombinedImageSampler;
		descWrites.pImageInfo = &imageInfo;

		Update({ descWrites });

#ifndef NDEBUG
		m_UniformBound = true;
#endif
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		m_DescriptorSet = other.m_DescriptorSet;
		m_SetNumber = other.m_SetNumber;
		m_PipelineBindPoint = other.m_PipelineBindPoint;
		m_PipelineLayout = other.m_PipelineLayout;
		return *this;
	}

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept
		: m_DescriptorSet(other.m_DescriptorSet), m_SetNumber(other.m_SetNumber),
		  m_PipelineBindPoint(other.m_PipelineBindPoint), m_PipelineLayout(other.m_PipelineLayout)
	{
	}
}  // namespace At0::Ray
