#include "Rpch.h"
#include "RTechnique.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	bool Technique::HasAttribute(const std::string& attribName) const
	{
		return m_Attributes.find(attribName) != m_Attributes.end();
	}

	bool Technique::HasUniformInBlock(
		std::string_view uniformBlockName, std::string_view uniformName) const
	{
		return m_BufferUniforms.find(uniformBlockName.data()) != m_BufferUniforms.end() &&
			   std::find(m_BufferUniforms.at(uniformBlockName.data()).begin(),
				   m_BufferUniforms.at(uniformBlockName.data()).end(),
				   uniformName) != m_BufferUniforms.at(uniformBlockName.data()).end();
	}

	bool Technique::HasSampler2DUniform(const std::string& uniformName)
	{
		return std::find(m_Sampler2DUniforms.begin(), m_Sampler2DUniforms.end(), uniformName) !=
			   m_Sampler2DUniforms.end();
	}

	bool Technique::HasVertexAttribute(const std::string& attribName)
	{
		return m_VertexAttributes.find(attribName) != m_Attributes.end();
	}

	void Technique::RequiresAttribute(std::string_view attribType, const std::string& attribName)
	{
		RAY_MEXPECTS(
			!HasAttribute(attribName), "[Technique] Attribute \"{0}\" already exists", attribName);

		m_Attributes[attribName] = attribType;
	}

	void Technique::RequiresBufferUniform(
		const std::string& uniformBlockName, std::string_view uniformName)
	{
		RAY_MEXPECTS(!HasUniformInBlock(uniformBlockName, uniformName),
			"[Technique] Uniform block \"{0}\" already has uniform \"{1}\"", uniformBlockName,
			uniformName);

		m_BufferUniforms[uniformBlockName].emplace_back(uniformName);
	}

	void Technique::RequiresSampler2DUniform(std::string_view uniformName)
	{
		RAY_MEXPECTS(!HasSampler2DUniform(uniformName.data()),
			"[Technique] Sampler2D uniform \"{0}\" already exists", uniformName);

		m_Sampler2DUniforms.emplace_back(uniformName);
	}

	void Technique::RequiresVertexAttribute(
		std::string_view attribType, const std::string& attribName)
	{
		RAY_MEXPECTS(!HasVertexAttribute(attribName),
			"[Technique] Vertex attribute \"{0}\" already exists", attribName);

		m_VertexAttributes[attribName] = attribType;
	}

	std::string Technique::MergeAttributes(uint32_t& location) const
	{
		std::string merged = "";
		for (const auto& child : m_ChildTechniques)
		{
			merged += child.second->MergeAttributes(location) + '\n';
		}

		for (const auto& [attribName, attribType] : m_Attributes)
		{
			if (!String::Contains(merged, attribName))
				merged += String::Serialize(
					"layout(location = {0}) in {1} {2};", location++, attribType, attribName);
		}

		return merged;
	}

	std::string Technique::MergeVertexAttributes(uint32_t& location) const { return std::string(); }

	std::string Technique::MergeUniforms(uint32_t& binding) const
	{
		std::string merged = "";
		for (const auto& child : m_ChildTechniques)
		{
			merged += child.second->MergeUniforms(binding) + '\n';
		}

		for (const auto& [uBlockName, uNames] : m_BufferUniforms)
		{
			std::string uniformStr = "";
			for (const std::string& uName : uNames)
				if (!String::Contains(uniformStr, uName))
					uniformStr += uName + ";\n";

			if (!String::Contains(merged, uBlockName))
				merged += String::Serialize("layout(set = 1, binding = {0}) uniform {1}\n{{2}\n}",
					binding++, uBlockName, uniformStr);
		}

		for (std::string_view uniformName : m_Sampler2DUniforms)
		{
			if (!String::Contains(merged, uniformName))
				merged += String::Serialize("layout(set = 1, binding = {0}) uniform sampler2D {1};",
					binding++, uniformName);
		}

		return merged;
	}

	void Technique::Connect(uint32_t pointID, Scope<Technique> tech)
	{
		m_ChildTechniques[pointID] = std::move(tech);
	}
}  // namespace At0::Ray
