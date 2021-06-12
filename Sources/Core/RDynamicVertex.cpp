#include "Rpch.h"
#include "RDynamicVertex.h"

#include "Graphics/Pipelines/Shader/RShader.h"
#include "Utils/RLogger.h"


namespace At0::Ray
{
	DynamicVertex::DynamicVertex(const Shader& shader)
	{
		if (!shader.GetAttributes(ShaderStage::Vertex))
		{
			Log::Warn("[DynamicVertex] Shader does not have attributes");
			return;
		}

		auto& attribs = *shader.GetAttributes(ShaderStage::Vertex);
		for (uint32_t i = 0; i < attribs.Size(); ++i)
		{
			m_AttribSizeMap.emplace_back(attribs[i].attributeName, attribs[i].size);
			m_SizeVertex += attribs[i].size;
		}
	}

	uint32_t DynamicVertex::SizeAttribute(std::string_view attribName) const
	{
		for (const auto& pair : m_AttribSizeMap)
			if (pair.first == attribName)
				return pair.second;

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

	bool DynamicVertex::Has(std::string_view attribName) const
	{
		for (const auto& pair : m_AttribSizeMap)
			if (pair.first == attribName)
				return true;
		return false;
	}

	std::vector<VkVertexInputAttributeDescription> DynamicVertex::GetVertexInputAttributes(
		uint32_t binding) const
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescs{};
		attributeDescs.reserve(m_AttribSizeMap.size());

		uint32_t i = 0;
		for (const auto& pair : m_AttribSizeMap)
		{
			VkVertexInputAttributeDescription attribDesc{};

			attribDesc.binding = binding;
			attribDesc.location = i;
			attribDesc.format = GetFormat(pair.first);
			attribDesc.offset = GetOffsetInSizeMap(pair.first);

			attributeDescs.emplace_back(attribDesc);
			++i;
		}

		return attributeDescs;
	}

	std::vector<VkVertexInputBindingDescription> DynamicVertex::GetVertexInputBindings(
		uint32_t binding) const
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = binding;
		bindingDescription.stride = SizeVertex();
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return { bindingDescription };
	}

	VkFormat DynamicVertex::GetFormat(std::string_view attribName) const
	{
		if (attribName == AttributeMap<AttributeType::Position>::Semantic)
			return AttributeMap<AttributeType::Position>::Format;
		else if (attribName == AttributeMap<AttributeType::UV>::Semantic)
			return AttributeMap<AttributeType::UV>::Format;
		else if (attribName == AttributeMap<AttributeType::Normal>::Semantic)
			return AttributeMap<AttributeType::Normal>::Format;
		else if (attribName == AttributeMap<AttributeType::Tangent>::Semantic)
			return AttributeMap<AttributeType::Tangent>::Format;

		return VK_FORMAT_UNDEFINED;
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
