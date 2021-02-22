#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "../Utils/RAssert.h"


namespace At0::Ray
{
	class VertexLayout
	{
	public:
		class Element
		{
		public:
			Element(VkFormat format, uint32_t offset) : m_Format(format), m_Offset(offset) {}

			VkFormat GetFormat() const { return m_Format; }
			uint32_t GetOffset() const { return m_Offset; }

		private:
			VkFormat m_Format = VK_FORMAT_UNDEFINED;

			// Specifies the offset in bytes in VertexInput::m_Data;
			uint32_t m_Offset = 0;
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
				size += SizeOf(elem.GetFormat());
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
				size += SizeOf(m_Elements.back().GetFormat());

			return size;
		}

		const Element& GetElement(uint32_t i) const { return m_Elements[i]; }

		/**
		 * @returns The size in bytes of the type associated to the format
		 */
		static uint32_t SizeOf(VkFormat format)
		{
			// RAY_TODO: Add more formats

			switch (format)
			{
			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R32_SINT:
			case VK_FORMAT_R32_SFLOAT: return 4;
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R32G32_SINT:
			case VK_FORMAT_R32G32_SFLOAT: return 2 * 4;
			case VK_FORMAT_R32G32B32_UINT:
			case VK_FORMAT_R32G32B32_SINT:
			case VK_FORMAT_R32G32B32_SFLOAT: return 3 * 4;
			}

			RAY_ASSERT(false, "[Vertex] Format {0} is not supported.", (uint32_t)format);
			return 0;
		}

		uint32_t GetOffsetInVertex(VkFormat format) const
		{
			uint32_t offset = 0;
			for (const Element& elem : m_Elements)
				offset += SizeOf(elem.GetFormat());

			return offset - SizeOf(m_Elements[0].GetFormat());
		}

		template<typename... Args>
		void Append(Args&&... args)
		{
			(m_Elements.emplace_back(args, Size()), ...);
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
				attributeDescs[i].format = m_Elements[i].GetFormat();
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
		char* m_StartAddress = nullptr;
		uint32_t m_Size = 0;
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
			uint32_t offset = prevSize + m_Layout.GetElement(run).GetOffset();
			memcpy(m_Data.data() + offset, &arg, sizeof(arg));
		}

	private:
		std::vector<char> m_Data;
		VertexLayout m_Layout;
	};
}  // namespace At0::Ray
