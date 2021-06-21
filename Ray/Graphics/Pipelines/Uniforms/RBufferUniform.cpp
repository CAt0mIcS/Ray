#include "Rpch.h"
#include "RBufferUniform.h"

#include "../Shader/RShader.h"
#include "../RPipeline.h"
#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	BufferUniform::BufferUniform(std::string_view name, ShaderStage stage, const Pipeline& pipeline)
		: m_Name(name)
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

	BufferUniform::AccessType BufferUniform::operator[](const std::string& name)
	{
		RAY_MEXPECTS(std::find_if(m_UniformInBlockOffsets.begin(), m_UniformInBlockOffsets.end(),
						 [&name](const auto& r) { return r.first == name; }) !=
						 m_UniformInBlockOffsets.end(),
			"[BufferUniform] Uniform \"{0}\" not present in uniform block \"{1}\"", name,
			GetName());

		uint32_t offset;
		for (const auto& pair : m_UniformInBlockOffsets)
			if (pair.first == name)
				offset = pair.second;

		return BufferUniform::AccessType{ m_UniformBuffer.get(), offset };
	}

	void BufferUniform::Update(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		m_UniformBuffer->Update(data, size, offset);
	}

	void BufferUniform::Setup(uint32_t bufferSize)
	{
		m_UniformBuffer = MakeScope<DynamicUniformBuffer>(bufferSize);
	}
}  // namespace At0::Ray
