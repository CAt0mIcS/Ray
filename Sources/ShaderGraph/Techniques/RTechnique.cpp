#include "Rpch.h"
#include "RTechnique.h"

#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	std::string Technique::GetInputAttributes() const
	{
		std::string merged;
		for (const auto& [connection, tech] : m_ChildTechniques)
			merged += '\n' + tech->GetInputAttributes();
		return merged;
	}

	std::string Technique::GetFunctions() const
	{
		std::string merged;
		for (const auto& [connection, tech] : m_ChildTechniques)
			merged += '\n' + tech->GetFunctions();
		return merged;
	}

	std::string Technique::GetFunctionCalls() const
	{
		std::string merged;
		for (const auto& [connection, tech] : m_ChildTechniques)
			merged += '\n' + tech->GetFunctionCalls();
		return merged;
	}

	std::string Technique::GetBufferUniforms() const
	{
		std::string merged;
		for (const auto& [connection, tech] : m_ChildTechniques)
			merged += '\n' + tech->GetBufferUniforms();
		return merged;
	}

	std::string Technique::GetSamplerUniforms() const
	{
		std::string merged;
		for (const auto& [connection, tech] : m_ChildTechniques)
			merged += '\n' + tech->GetSamplerUniforms();
		return merged;
	}

	bool Technique::HasAttribute(const std::string& attribName) const
	{
		return m_Attributes.find(attribName) != m_Attributes.end();
	}

	bool Technique::HasUniformInBlock(
		const std::string& uniformBlockName, const std::string& uniformName) const
	{
		return m_BufferUniforms.find(uniformBlockName) != m_BufferUniforms.end() &&
			   m_BufferUniforms.at(uniformBlockName).find(uniformName) !=
				   m_BufferUniforms.at(uniformBlockName).end();
	}

	bool Technique::HasSampler2DUniform(const std::string& uniformName) const
	{
		return std::find(m_Sampler2DUniforms.begin(), m_Sampler2DUniforms.end(), uniformName) !=
			   m_Sampler2DUniforms.end();
	}

	bool Technique::HasVertexInputAttribute(const std::string& attribName) const
	{
		return m_VertexInputAttributes.find(attribName) != m_VertexInputAttributes.end();
	}

	bool Technique::HasVertexOutputAttribute(const std::string& attribName) const
	{
		return m_VertexOutputAttributes.find(attribName) != m_VertexOutputAttributes.end();
	}

	bool Technique::HasVertexAsignment(
		const std::string& outAttribName, const std::string& inAttribName) const
	{
		return m_VertexAssignments.find(outAttribName) != m_VertexAssignments.end() &&
			   m_VertexAssignments.at(outAttribName) == inAttribName;
	}

	void Technique::RequiresAttribute(std::string_view attribType, const std::string& attribName)
	{
		RAY_MEXPECTS(
			!HasAttribute(attribName), "[Technique] Attribute \"{0}\" already exists", attribName);

		m_Attributes[attribName] = attribType;
	}

	void Technique::RequiresBufferUniform(const std::string& uniformBlockName,
		std::string_view uniformType, const std::string& uniformName)
	{
		RAY_MEXPECTS(!HasUniformInBlock(uniformBlockName, uniformName.data()),
			"[Technique] Uniform block \"{0}\" already has uniform \"{1}\"", uniformBlockName,
			uniformName);

		m_BufferUniforms[uniformBlockName][uniformName] = uniformType;
	}

	void Technique::RequiresSampler2DUniform(std::string_view uniformName)
	{
		RAY_MEXPECTS(!HasSampler2DUniform(uniformName.data()),
			"[Technique] Sampler2D uniform \"{0}\" already exists", uniformName);

		m_Sampler2DUniforms.emplace_back(uniformName);
	}

	void Technique::RequiresVertexInputAttribute(
		std::string_view attribType, const std::string& attribName)
	{
		RAY_MEXPECTS(!HasVertexInputAttribute(attribName),
			"[Technique] Vertex attribute \"{0}\" already exists", attribName);

		m_VertexInputAttributes[attribName] = attribType;
	}

	void Technique::RequiresVertexOutputAttribute(
		std::string_view attribType, const std::string& attribName)
	{
		RAY_MEXPECTS(!HasVertexOutputAttribute(attribName),
			"[Technique] Vertex attribute \"{0}\" already exists", attribName);

		m_VertexOutputAttributes[attribName] = attribType;
	}

	void Technique::RequiresVertexAssignment(
		const std::string& outAttribName, const std::string& inAttribName)
	{
		RAY_MEXPECTS(!HasVertexAsignment(outAttribName, inAttribName),
			"[Technique] Vertex asignment of \"{0} = {1}\" already exists", outAttribName,
			inAttribName);

		m_VertexAssignments[outAttribName] = inAttribName;
	}

	std::string Technique::MergeAttributes(uint32_t& location) const
	{
		std::string merged = "";
		for (const auto& child : m_ChildTechniques)
		{
			merged += child.second->MergeAttributes(location);
		}

		for (const auto& [attribName, attribType] : m_Attributes)
		{
			if (!String::Contains(merged, attribName))
				merged += String::Serialize(
					"\nlayout(location = {0}) in {1} {2};", location++, attribType, attribName);
		}

		return merged;
	}

	std::string Technique::MergeUniforms(uint32_t& binding) const
	{
		std::string merged = "";
		for (const auto& child : m_ChildTechniques)
		{
			merged += child.second->MergeUniforms(binding) + '\n';
		}

		for (const auto& [uBlockName, uniforms] : m_BufferUniforms)
		{
			std::string uniformStr = "";
			for (const auto& [uName, uType] : uniforms)
				if (!String::Contains(uniformStr, uName))
					uniformStr += uType + ' ' + uName + ';';

			if (!String::Contains(merged, uBlockName))
				merged += String::Serialize(
					"\nlayout(set = 1, binding = {0}) uniform {1}\n{\n{2}\n} u{1};", binding++,
					uBlockName, uniformStr);
		}

		for (std::string_view uniformName : m_Sampler2DUniforms)
		{
			if (!String::Contains(merged, uniformName))
				merged +=
					String::Serialize("\nlayout(set = 1, binding = {0}) uniform sampler2D {1};",
						binding++, uniformName);
		}

		return merged;
	}

	std::string Technique::MergeVertexAttributes(
		uint32_t& vertexInputLocation, uint32_t& vertexOutputLocation) const
	{
		std::string merged = "";
		for (const auto& child : m_ChildTechniques)
		{
			merged +=
				child.second->MergeVertexAttributes(vertexInputLocation, vertexOutputLocation);
		}

		for (const auto& [attribName, attribType] : m_VertexInputAttributes)
		{
			if (!String::Contains(merged, attribName))
				merged += String::Serialize("\nlayout(location = {0}) in {1} {2};",
					vertexInputLocation++, attribType, attribName);
		}

		for (const auto& [attribName, attribType] : m_VertexOutputAttributes)
		{
			if (!String::Contains(merged, attribName))
				merged += String::Serialize("\nlayout(location = {0}) out {1} {2};",
					vertexOutputLocation++, attribType, attribName);
		}

		return merged;
	}

	std::string Technique::MergeVertexAssignments() const
	{
		std::string merged;
		for (const auto& child : m_ChildTechniques)
		{
			merged += child.second->MergeVertexAssignments() + '\n';
		}

		for (const auto& [outAttrib, inAttrib] : m_VertexAssignments)
		{
			merged += String::Serialize("\n{0} = {1};", outAttrib, inAttrib);
		}

		return merged;
	}

	void Technique::Connect(uint32_t pointID, Scope<Technique> tech)
	{
		m_ChildTechniques[pointID] = std::move(tech);
	}
}  // namespace At0::Ray
