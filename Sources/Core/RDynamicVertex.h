#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"


namespace At0::Ray
{
	class Shader;

	class RAY_EXPORT DynamicVertex
	{
	private:
		class ProxyType
		{
		public:
			ProxyType(char* writeLocation, uint32_t expectedDataSize)
				: m_WriteLocation(writeLocation), m_ExpectedSize(expectedDataSize)
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

		private:
			char* m_WriteLocation;
			uint32_t m_ExpectedSize = 0;
		};

	public:
		DynamicVertex(const Shader& shader);

		/**
		 * @returns Size in bytes of one vertex
		 */
		uint32_t SizeVertex() const { return m_SizeVertex; }

		/**
		 * @returns Size in bytes of the attribute with the name
		 */
		uint32_t SizeAttribute(std::string_view attribName) const;

		/**
		 * Reallocates the buffer to fit the data of one new vertex
		 * @returns Vertex ID of the vertex we're filling
		 */
		uint32_t BeginVertex();

		ProxyType operator[](std::string_view attribName);

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
