#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"
#include "../Graphics/Pipelines/Shader/RShaderTypes.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Shader;

	// Defines the tags used to identify position, normal, UV, ... in the shader
	template<AttributeType type>
	struct AttributeMap;
	template<>
	struct AttributeMap<AttributeType::Position>
	{
		static constexpr const char* Semantic = "inPos";
		static constexpr const char* Type = "vec3";
		static constexpr VkFormat Format = VK_FORMAT_R32G32B32_SFLOAT;
	};
	template<>
	struct AttributeMap<AttributeType::UV>
	{
		static constexpr const char* Semantic = "inUV";
		static constexpr const char* Type = "vec2";
		static constexpr VkFormat Format = VK_FORMAT_R32G32_SFLOAT;
	};
	template<>
	struct AttributeMap<AttributeType::Normal>
	{
		static constexpr const char* Semantic = "inNormal";
		static constexpr const char* Type = "vec3";
		static constexpr VkFormat Format = VK_FORMAT_R32G32B32_SFLOAT;
	};
	template<>
	struct AttributeMap<AttributeType::Tangent>
	{
		static constexpr const char* Semantic = "inTangent";
		static constexpr const char* Type = "vec3";
		static constexpr VkFormat Format = VK_FORMAT_R32G32B32_SFLOAT;
	};

	class RAY_EXPORT DynamicVertex
	{
		friend class ProxyType;

	private:
		class ProxyType
		{
		public:
			ProxyType(
				char* writeLocation, DynamicVertex* pVertex, RAY_DEBUG_FLAG(uint32_t expectedSize))
				: m_WriteLocation(writeLocation),
				  m_Vertex(pVertex) RAY_DEBUG_FLAG(, m_ExpectedSize(expectedSize))
			{
			}

			template<typename T>
			ProxyType& operator=(T&& data)
			{
				RAY_MEXPECTS(sizeof(data) == m_ExpectedSize,
					"[DynamicVertex] Data begin uploaded to buffer "
					"does not meet size requirements. Expected {0} byte(s), received {1} byte(s)",
					m_ExpectedSize, sizeof(data));
				memcpy(m_WriteLocation, &data, sizeof(data));
				return *this;
			}

			template<typename T>
			T& Get(std::string_view attribName)
			{
				RAY_MEXPECTS(sizeof(T) == m_Vertex->SizeAttribute(attribName),
					"[DynamicVertex] Size of return type does not match size of attribName. "
					"Expected {0} byte(s), received {1} byte(s)",
					m_Vertex->SizeAttribute(attribName), sizeof(T));

				return *(T*)(m_WriteLocation + m_Vertex->GetOffsetInSizeMap(attribName));
			}

		private:
			char* m_WriteLocation;
			DynamicVertex* m_Vertex = nullptr;
			RAY_DEBUG_FLAG(uint32_t m_ExpectedSize = 0);
		};

	public:
		DynamicVertex(const Shader& shader);
		DynamicVertex(std::vector<std::pair<std::string, uint32_t>> attribSizeMap);

		/**
		 * @returns Size in bytes of one vertex
		 */
		uint32_t SizeVertex() const { return m_SizeVertex; }

		/**
		 * @returns Size in bytes of all the vertices
		 */
		uint32_t SizeBytes() const { return m_Buffer.size(); }

		/**
		 * @returns The number of vertices added
		 */
		uint32_t Size() const { return SizeBytes() / SizeVertex(); }

		/**
		 * @returns Raw buffer data
		 */
		const char* Data() const { return m_Buffer.data(); }

		/**
		 * @returns Size in bytes of the attribute with the name
		 */
		uint32_t SizeAttribute(std::string_view attribName) const;

		/**
		 * Reallocates the buffer to fit the data of one new vertex
		 * @returns Vertex ID of the vertex we're filling
		 */
		uint32_t BeginVertex();

		/**
		 * Set attribute with attribName
		 */
		ProxyType operator[](std::string_view attribName);

		/**
		 * Gets vertex with specific vertex id
		 */
		ProxyType operator[](uint32_t vertexID);

		/**
		 * Adds vertices of vertex to this. Requires matching layouts
		 */
		void AddAll(const DynamicVertex& vertex);

		/**
		 * @returns True if the attribute name is present in the shader
		 */
		bool Has(std::string_view attribName) const;

		/**
		 * @returns the layout of the vertex
		 */
		const auto& GetLayout() const { return m_AttribSizeMap; }

		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributes(
			uint32_t binding = 0) const;

		std::vector<VkVertexInputBindingDescription> GetVertexInputBindings(
			uint32_t binding = 0) const;

		VkFormat GetFormat(std::string_view attribName) const;

	private:
		/**
		 * @returns The offset of the attribute in the attribute map
		 */
		uint32_t GetOffsetInSizeMap(std::string_view attribName) const;

	private:
		std::vector<char> m_Buffer;

		/**
		 * Specifies the attribute name ("inPos") and the size in bytes of the attribute
		 */
		std::vector<std::pair<std::string, uint32_t>> m_AttribSizeMap;

		uint32_t m_SizeVertex = 0;

		/**
		 * Specifies the current vertex we're filling. Incremented in BeginVertex and used as an
		 * offset calculator into the buffer
		 */
		uint32_t m_VertexID = (uint32_t)-1;
	};
}  // namespace At0::Ray
