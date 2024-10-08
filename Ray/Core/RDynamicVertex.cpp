﻿#include "RDynamicVertex.h"

#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/Shader/RShaderReflection.h"


namespace At0::Ray
{
	DynamicVertex::DynamicVertex(const Shader& shader)
	{
		if (shader.GetReflection(ShaderStage::Vertex).GetAttributes().size() == 0)
		{
			Log::Warn("[DynamicVertex] Shader does not have attributes");
			return;
		}

		auto& attribs = shader.GetReflection(ShaderStage::Vertex).GetAttributes();
		for (uint32_t i = 0; i < attribs.size(); ++i)
		{
			m_AttribSizeMap.emplace_back(attribs[i].name, attribs[i].size);
			m_SizeVertex += attribs[i].size;
		}
	}

	DynamicVertex::DynamicVertex(std::vector<std::pair<std::string, uint32_t>> attribSizeMap)
		: m_AttribSizeMap(std::move(attribSizeMap))
	{
		for (const auto& [attribName, attribSize] : m_AttribSizeMap)
			m_SizeVertex += attribSize;
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
			this RAY_DEBUG_FLAG(, SizeAttribute(attribName)) };
	}

	DynamicVertex::ProxyType DynamicVertex::operator[](uint32_t vertexID)
	{
		return { m_Buffer.data() + (m_SizeVertex * vertexID), this RAY_DEBUG_FLAG(, 0) };
	}

	void DynamicVertex::AddAll(const DynamicVertex& vertex)
	{
		uint32_t offset = m_Buffer.size();
		m_Buffer.resize(m_Buffer.size() + vertex.SizeBytes());
		memcpy(m_Buffer.data() + offset, vertex.Data(), vertex.SizeBytes());

		m_VertexID += vertex.Size();
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
		else if (attribName == AttributeMap<AttributeType::Position2D>::Semantic)
			return AttributeMap<AttributeType::Position2D>::Format;
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
