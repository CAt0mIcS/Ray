#include "Rpch.h"
#include "RShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	void ShaderGenerator::AddInputAttribute(
		uint32_t location, std::string_view name, Shader::DataType type)
	{
		m_Source += String::Serialize(
			"layout(location = {0}) in {1} {2};\n", location, String::Construct(type), name);
	}

	void ShaderGenerator::AddOutputAttribute(
		uint32_t location, std::string_view name, Shader::DataType type)
	{
		m_Source += String::Serialize(
			"layout(location = {0}) out {1} {2};\n", location, String::Construct(type), name);
	}

	void ShaderGenerator::AddUniform(uint32_t set, std::string_view blockName,
		const UniformLayout& data, std::string_view instanceName)
	{
		m_Source += String::Serialize(
			"layout(set = {0}, binding = {1}) uniform {2}\n{\n", set, m_NextBinding++, blockName);

		for (const Member& member : data)
		{
			m_Source +=
				String::Serialize("\t{0} {1};\n", String::Construct(member.type), member.name);
		}

		m_Source += String::Serialize("} {0};\n", instanceName);
	}

	void ShaderGenerator::AddFunction(std::string_view fName, std::string_view mainCode,
		std::string_view retType, std::string_view args)
	{
		m_Source += String::Serialize("{0} {1}({2})\n{\n{3}\n}", retType, fName, args, mainCode);
	}
}  // namespace At0::Ray
