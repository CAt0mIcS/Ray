#include "Rpch.h"
#include "RDynamicVertex.h"

#include "Graphics/Pipelines/RShader.h"
#include "Utils/RLogger.h"


namespace At0::Ray
{
	DynamicVertex::DynamicVertex(const Shader& shader)
	{
		if (!shader.GetAttributes(Shader::Stage::Vertex))
		{
			Log::Warn("[DynamicVertex] Shader does not have attributes");
			return;
		}

		for (const auto& attrib : (*shader.GetAttributes(Shader::Stage::Vertex)))
		{
			m_AttribSizeMap.emplace_back(attrib.attributeName, attrib.size);
			m_SizeVertex += attrib.size;
		}
	}

	uint32_t DynamicVertex::SizeAttribute(std::string_view attribName) const
	{
		for (const auto& pair : m_AttribSizeMap)
		{
			if (pair.first == attribName)
				return pair.second;
		}

		RAY_ASSERT(false, "[DynamicVertex] Attribute \"{0}\" not found", attribName);
		return 0;
	}

	uint32_t DynamicVertex::BeginVertex()
	{
		m_Buffer.resize(m_Buffer.size() + SizeVertex());
		return ++m_VertexID;
	}

	DynamicVertex::ProxyType DynamicVertex::operator[](std::string_view attribName)
	{
		return { m_Buffer.data() + (m_SizeVertex * m_VertexID) + GetOffsetInSizeMap(attribName),
			SizeAttribute(attribName) };
	}

	uint32_t DynamicVertex::GetOffsetInSizeMap(std::string_view attribName) const
	{
		uint32_t offset = 0;
		for (const auto& pair : m_AttribSizeMap)
		{
			if (pair.first == attribName)
				return offset;
			offset += pair.second;
		}

		return offset;
	}
}  // namespace At0::Ray
