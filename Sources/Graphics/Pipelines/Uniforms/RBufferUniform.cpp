#include "Rpch.h"
#include "RBufferUniform.h"

#include "../RPipeline.h"
#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	BufferUniform::BufferUniform(
		std::string_view name, Shader::Stage stage, const Pipeline& pipeline)
		: m_Name(name)
	{
		RAY_MEXPECTS(pipeline.GetShader().HasUniform(name, stage),
			"[BufferUniform] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		auto uniformBlock = pipeline.GetShader().GetUniformBlocks(stage)->Get(name);
		m_Binding = uniformBlock->binding;

		// Fill map with offsets of all uniforms in uniform block
		for (auto& uniform : uniformBlock->uniforms)
			m_UniformInBlockOffsets[uniform.uniformName] = uniform.offset;

		Setup(uniformBlock->size);
	}

	BufferUniform::BufferUniform(std::string_view name, uint32_t binding, uint32_t size,
		std::unordered_map<std::string, uint32_t> uniformInBlockOffsets)
		: m_Name(name), m_Binding(binding),
		  m_UniformInBlockOffsets(std::move(uniformInBlockOffsets))
	{
		Setup(size);
	}

	uint32_t BufferUniform::GetOffset() const
	{
		return BufferSynchronizer::Get().GetUniformBuffer().GetOffset(m_Offset);
	}

	const Buffer& BufferUniform::GetBuffer() const
	{
		return BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(m_Offset);
	}

	BufferUniform::ProxyType BufferUniform::operator[](std::string_view name)
	{
		RAY_MEXPECTS(m_UniformInBlockOffsets.find(name.data()) != m_UniformInBlockOffsets.end(),
			"[BufferUniform] Uniform \"{0}\" not present in uniform block \"{1}\"", name,
			GetName());

		return BufferUniform::ProxyType{ this, m_UniformInBlockOffsets[name.data()] };
	}

	void BufferUniform::Setup(uint32_t bufferSize)
	{
		m_Size = bufferSize;

		BufferSynchronizer::Get().Emplace(bufferSize, &m_Offset);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(m_Offset);
		bufferInfo.offset = BufferSynchronizer::Get().GetUniformBuffer().GetOffset(m_Offset);
		bufferInfo.range = m_Size;
	}
}  // namespace At0::Ray
