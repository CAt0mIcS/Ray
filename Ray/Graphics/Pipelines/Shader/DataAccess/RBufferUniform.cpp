#include "Rpch.h"
#include "RBufferUniform.h"

#include "Graphics/Commands/RCommandBuffer.h"
#include "../RShader.h"
#include "../../RPipeline.h"
#include "../RShaderReflection.h"
#include "Graphics/Buffers/Dynamic/RDynamicUniformBuffer.h"
#include "Graphics/Buffers/RBuffer.h"

#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	BufferUniform::BufferUniform(std::string_view name, ShaderStage stage, const Pipeline& pipeline)
		: m_Name(name), m_ShaderStage(stage)
	{
		RAY_MEXPECTS(pipeline.GetShader().GetReflection(stage).HasUniformBlock(name),
			"[BufferUniform] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		auto& uniformBlock = pipeline.GetShader().GetReflection(stage).GetUniformBlock(name);
		m_Binding = uniformBlock.binding;

		// Fill map with offsets of all uniforms in uniform block
		for (auto& uniform : uniformBlock.uniforms)
			m_UniformInBlockOffsets.emplace_back(std::pair{ uniform.name, uniform.offset });

		Setup(uniformBlock.size);
	}

	BufferUniform::BufferUniform(std::string_view name, uint32_t binding, uint32_t size,
		std::unordered_map<std::string, uint32_t> uniformInBlockOffsets)
		: m_Name(name), m_Binding(binding)
	{
		for (const auto& [name, offset] : uniformInBlockOffsets)
			m_UniformInBlockOffsets.emplace_back(std::pair{ name, offset });

		Setup(size);
	}

	BufferUniform::~BufferUniform() {}

	uint32_t BufferUniform::GetOffset() const { return m_UniformBuffer->GetOffset(); }

	uint32_t BufferUniform::GetSize() const { return m_UniformBuffer->GetSize(); }

	DynamicBufferAccess BufferUniform::operator[](const std::string& name)
	{
		RAY_MEXPECTS(Has(name),
			"[BufferUniform] Uniform \"{0}\" not present in uniform block \"{1}\"", name,
			GetName());

		uint32_t offset;
		for (const auto& pair : m_UniformInBlockOffsets)
			if (pair.first == name)
			{
				offset = pair.second;
				break;
			}

		return DynamicBufferAccess{ m_UniformBuffer.get(), offset };
	}

	void BufferUniform::Update(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		m_UniformBuffer->Update(data, size, offset);
	}

	bool BufferUniform::Has(const std::string& name)
	{
		return std::find_if(m_UniformInBlockOffsets.begin(), m_UniformInBlockOffsets.end(),
				   [&name](const auto& r)
				   { return r.first == name; }) != m_UniformInBlockOffsets.end();
	}

	void BufferUniform::CmdBindAsPushConstant(
		const CommandBuffer& cmdBuff, VkPipelineLayout pipelineLayout) const
	{
		vkCmdPushConstants(cmdBuff, pipelineLayout, (VkShaderStageFlags)m_ShaderStage, 0,
			m_UniformBuffer->GetSize(),
			((char*)m_UniformBuffer->GetBuffer().GetMapped()) + m_UniformBuffer->GetOffset());
	}

	BufferUniform::BufferUniform(BufferUniform&& other) noexcept { *this = std::move(other); }

	BufferUniform& BufferUniform::operator=(BufferUniform&& other) noexcept
	{
		m_Name = std::move(other.m_Name);
		m_Binding = other.m_Binding;
		m_UniformBuffer = std::move(other.m_UniformBuffer);
		m_UniformInBlockOffsets = std::move(other.m_UniformInBlockOffsets);
		m_ShaderStage = other.m_ShaderStage;
		return *this;
	}

	void BufferUniform::Setup(uint32_t bufferSize)
	{
		m_UniformBuffer = MakeScope<DynamicUniformBuffer>(bufferSize);
	}
}  // namespace At0::Ray
