#include "Rpch.h"
#include "RTechnique.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	bool Technique::HasAttribute(std::string_view attribName) const
	{
		return m_Attributes.find(attribName.data()) != m_Attributes.end();
	}

	bool Technique::HasUniformInBlock(
		std::string_view uniformBlockName, std::string_view uniformName) const
	{
		return m_BufferUniforms.find(uniformBlockName.data()) != m_BufferUniforms.end() &&
			   std::find(m_BufferUniforms.at(uniformBlockName.data()).begin(),
				   m_BufferUniforms.at(uniformBlockName.data()).end(),
				   uniformName) != m_BufferUniforms.at(uniformBlockName.data()).end();
	}

	bool Technique::HasSampler2DUniform(std::string_view uniformName)
	{
		return std::find(m_Sampler2DUniforms.begin(), m_Sampler2DUniforms.end(), uniformName) !=
			   m_Sampler2DUniforms.end();
	}

	void Technique::RequiresAttribute(std::string_view attribType, std::string_view attribName)
	{
		RAY_MEXPECTS(
			!HasAttribute(attribName), "[Technique] Attribute \"{0}\" already exists", attribName);
	}

	void Technique::RequiresBufferUniform(
		std::string_view uniformBlockName, std::string_view uniformName)
	{
		RAY_MEXPECTS(!HasUniformInBlock(uniformBlockName, uniformName),
			"[Technique] Uniform block \"{0}\" already has uniform \"{1}\"", uniformBlockName,
			uniformName);

		m_BufferUniforms[uniformBlockName.data()].emplace_back(uniformName);
	}

	void Technique::RequiresSampler2DUniform(std::string_view uniformName)
	{
		RAY_MEXPECTS(!HasSampler2DUniform(uniformName),
			"[Technique] Sampler2D uniform \"{0}\" already exists", uniformName);

		m_Sampler2DUniforms.emplace_back(uniformName);
	}

	void Technique::Connect(uint32_t pointID, Scope<Technique> tech)
	{
		m_ChildTechniques[pointID] = std::move(tech);
	}
}  // namespace At0::Ray
