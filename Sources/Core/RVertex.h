#pragma once

#include <vector>

#include "../Utils/RAssert.h"


namespace At0::Ray
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Float3Color,
			Float4Color,
		};

		class Element
		{
		public:
			Element(ElementType type, uint32_t offset) : m_Type(type), m_Offset(offset) {}

			ElementType GetType() const { return m_Type; }
			uint32_t GetOffset() const { return m_Offset; }

		private:
			ElementType m_Type;

			// Specifies the offset in bytes in VertexInput::m_Data;
			uint32_t m_Offset;
		};

	public:
		/**
		 * @returns The size in bytes of a vertex
		 */
		uint32_t SizeVertex() const
		{
			int32_t size = 0;
			for (const Element& elem : m_Elements)
			{
				size += SizeOf(elem.GetType());
			}

			return size;
		}

		/**
		 * @returns The number of elements
		 */
		uint32_t NumberOfElements() const { return m_Elements.size(); }

		/**
		 * @returns The size in bytes of all of the elements
		 */
		uint32_t Size() const
		{
			uint32_t size = 0;
			for (const Element& elem : m_Elements)
			{
				size += elem.GetOffset();
			}

			if (m_Elements.size() > 0)
				size += SizeOf(m_Elements.back().GetType());

			return size;
		}

		const Element& GetElement(uint32_t i) const { return m_Elements[i]; }

		/**
		 * @returns The size in bytes of the type associated to the element type
		 */
		uint32_t SizeOf(ElementType type) const
		{
			switch (type)
			{
			case Position2D: return sizeof(Float2);
			case Position3D: return sizeof(Float3);
			case Float3Color: return sizeof(Float3);
			case Float4Color: return sizeof(Float4);
			}

			RAY_ASSERT(false, "[Vertex] Element type {0} is invalid", (uint32_t)type);
			return 0;
		}

		uint32_t GetOffsetInVertex(ElementType type) const
		{
			uint32_t offset = 0;
			for (auto it = m_Elements.begin(); it->GetType() != type && it != m_Elements.end();
				 ++it)
			{
				offset += SizeOf(it->GetType());
			}

			return offset;
		}

		template<typename... Args>
		void Append(Args&&... args)
		{
			(m_Elements.emplace_back(args, Size()), ...);
		}

		VkFormat GetFormat(ElementType type) const
		{
			switch (type)
			{
			case Position2D: return VK_FORMAT_R32G32_SFLOAT;
			case Position3D: return VK_FORMAT_R32G32B32_SFLOAT;
			// case Texture2D: return VK_FORMAT_R32G32_SFLOAT;
			// case Normal: return VK_FORMAT_R32G32B32_SFLOAT;
			// case Tangent: return ;
			// case Bitangent: return ;
			case Float3Color: return VK_FORMAT_R32G32B32_SFLOAT;
			case Float4Color:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
				// case Count: return VK_FORMAT_UNDEFINED;
			}

			RAY_ASSERT(false, "[Vertex] Type {0} is invalid", (uint32_t)type);
			return VK_FORMAT_UNDEFINED;
		}

		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions(
			std::vector<uint32_t> locations = {}, uint32_t binding = 0) const
		{
			std::vector<VkVertexInputAttributeDescription> attributeDescs{};
			attributeDescs.resize(m_Elements.size());

			if (locations.empty())
			{
				for (uint32_t i = 0; i < attributeDescs.size(); ++i)
					locations.emplace_back(i);
			}

			for (uint32_t i = 0; i < m_Elements.size(); ++i)
			{
				attributeDescs[i].binding = binding;
				attributeDescs[i].location = locations[i];
				attributeDescs[i].format = GetFormat(m_Elements[i].GetType());
				attributeDescs[i].offset = m_Elements[i].GetOffset();
			}

			return attributeDescs;
		}

		std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions(
			uint32_t binding) const
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = binding;
			bindingDescription.stride = Size();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return { bindingDescription };
		}

	private:
		// Specifies all the elements in a vertex
		std::vector<Element> m_Elements;
	};


	class Vertex
	{
	public:
		Vertex(char* startAddress, uint32_t size) : m_StartAddress(startAddress), m_Size(size) {}

	private:
		char* m_StartAddress;
		uint32_t m_Size;
	};


	class VertexInput
	{
	public:
		VertexInput(VertexLayout layout) : m_Layout(layout) {}

		template<typename... Args>
		void Emplace(Args&&... args)
		{
			RAY_MEXPECTS(sizeof...(args) == m_Layout.NumberOfElements(),
				"[VertexInput] Number of arguments ({0}) must match number of layout elements "
				"({1}).",
				sizeof...(args), m_Layout.NumberOfElements());

			// Resize the buffer to hold the new vertex
			uint32_t prevSize = (uint32_t)m_Data.size();
			m_Data.resize(m_Data.size() + m_Layout.SizeVertex());

			int run = 0;
			(Insert(prevSize, run++, std::forward<Args>(args)), ...);
		}

		uint32_t SizeBytes() const { return m_Data.size(); }
		const char* Data() const { return m_Data.data(); }

	private:
		template<typename T>
		void Insert(uint32_t prevSize, int run, T&& arg)
		{
			uint32_t elemOffset = m_Layout.GetOffsetInVertex(m_Layout.GetElement(run).GetType());
			memcpy(m_Data.data() + prevSize + elemOffset, &arg, sizeof(arg));
		}

	private:
		std::vector<char> m_Data;
		VertexLayout m_Layout;
	};
}  // namespace At0::Ray
