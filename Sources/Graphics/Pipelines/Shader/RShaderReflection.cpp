#include "Rpch.h"
#include "RShaderReflection.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	ShaderReflection::ShaderReflection(std::string_view filepath) {}

	void ShaderReflection::AddUniformBlock(UniformBlockData data)
	{
		m_UniformBlocks.emplace_back(std::move(data));
	}

	bool ShaderReflection::HasAttribute(std::string_view name) const
	{
		return std::find_if(m_Attributes.begin(), m_Attributes.end(),
				   [&name](const AttributeData& data) { return data.name == name; }) !=
			   m_Attributes.end();
	}

	bool ShaderReflection::HasUniform(std::string_view name, bool includeUniformBlocks) const
	{
		if (includeUniformBlocks)
			for (const auto& uBlockData : m_UniformBlocks)
				for (const auto& uData : uBlockData.uniforms)
					if (uData.name == name)
						return true;

		return std::find_if(m_Uniforms.begin(), m_Uniforms.end(), [&name](const UniformData& data) {
			return data.name == name;
		}) != m_Uniforms.end();
	}

	bool ShaderReflection::HasUniformBlock(std::string_view name) const
	{
		return std::find_if(m_UniformBlocks.begin(), m_UniformBlocks.end(),
				   [&name](const UniformBlockData& data) { return data.name == name; }) !=
			   m_UniformBlocks.end();
	}

	const ShaderReflection::AttributeData& ShaderReflection::GetAttribute(
		std::string_view name) const
	{
		for (const auto& attrib : m_Attributes)
			if (attrib.name == name)
				return attrib;

		ThrowRuntime("[ShaderReflection] Failed to get attribute with name \"{0}\"", name);
		return m_Attributes[0];
	}

	const ShaderReflection::UniformData& ShaderReflection::GetUniform(std::string_view name) const
	{
		for (const auto& uniform : m_Uniforms)
			if (uniform.name == name)
				return uniform;

		ThrowRuntime("[ShaderReflection] Failed to get uniform with name \"{0}\"", name);
		return m_Uniforms[0];
	}

	const ShaderReflection::UniformBlockData& ShaderReflection::GetUniformBlock(
		std::string_view name) const
	{
		for (const auto& uniformBlock : m_UniformBlocks)
			if (uniformBlock.name == name)
				return uniformBlock;

		ThrowRuntime("[ShaderReflection] Failed to get uniform block with name \"{0}\"", name);
		return m_UniformBlocks[0];
	}

	ShaderReflection::AttributeData& ShaderReflection::GetAttribute(std::string_view name)
	{
		return const_cast<AttributeData&>(std::as_const(*this).GetAttribute(name));
	}

	ShaderReflection::UniformData& ShaderReflection::GetUniform(std::string_view name)
	{
		return const_cast<UniformData&>(std::as_const(*this).GetUniform(name));
	}

	ShaderReflection::UniformBlockData& ShaderReflection::GetUniformBlock(std::string_view name)
	{
		return const_cast<UniformBlockData&>(std::as_const(*this).GetUniformBlock(name));
	}

	const ShaderReflection::UniformData& ShaderReflection::UniformBlockData::GetUniform(
		std::string_view name) const
	{
		for (const auto& uniform : uniforms)
			if (uniform.name == name)
				return uniform;

		ThrowRuntime("[ShaderReflection] Failed to get uniform with name \"{0}\"", name);
		return uniforms[0];
	}

	ShaderReflection::UniformData& ShaderReflection::UniformBlockData::GetUniform(
		std::string_view name)
	{
		return const_cast<UniformData&>(std::as_const(*this).GetUniform(name));
	}
}  // namespace At0::Ray
