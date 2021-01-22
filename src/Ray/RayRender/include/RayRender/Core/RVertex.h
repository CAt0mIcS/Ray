#pragma once

#include <vector>

#include <../../RayUtil/include/RayUtil/RMath.h>
#include <../../RayDebug/include/RayDebug/RAssert.h>


namespace At0::Ray
{
	struct BGRAColor
	{
		uint8_t a;
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	// Describes what VertexData::m_Data looks like
	class VertexLayout
	{
		class Element;
	public:
		enum ElementType : uint8_t
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};

	public:
		VertexLayout()
		{
		}

		template<ElementType Type> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = Float2;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Position3D>
		{
			using SysType = Float3;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Texture2D>
		{
			using SysType = Float2;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Normal>
		{
			using SysType = Float3;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Tangent>
		{
			using SysType = Float3;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Bitangent>
		{
			using SysType = Float3;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Float3Color>
		{
			using SysType = Float3;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Float4Color>
		{
			using SysType = Float4;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = ::At0::Ray::BGRAColor;
			static constexpr size_t Size = sizeof(SysType);
		};
		template<> struct Map<Count>
		{
			using SysType = long double;
			static constexpr size_t Size = sizeof(SysType);
		};

		/// <returns>The size of the struct associated with the ElementType</returns>
		static constexpr uint32_t SizeOf(ElementType type)
		{
			switch (type)
			{
			case Position2D:	return sizeof(Float2);
			case Position3D:	return sizeof(Float3);
			case Texture2D:		return sizeof(Float2);
			case Normal:		return sizeof(Float3);
			case Tangent:		return sizeof(Float3);
			case Bitangent:		return sizeof(Float3);
			case Float3Color:	return sizeof(Float3);
			case Float4Color:	return sizeof(Float4);
			case BGRAColor:		return sizeof(::At0::Ray::BGRAColor);
			case Count:			return sizeof(long double);
			}

			RAY_ASSERT(false, "[VertexLayout::SizeOf] Type (ID={0}) is invalid.", (uint8_t)type);
			return 0;
		}

		/// <summary>
		/// Adds a new ElementType to the dynamic vertex
		/// </summary>
		template<ElementType... Type>
		VertexLayout& Append()
		{
			(m_Elements.emplace_back(Type, QueryNextOffset()), ...);
			return *this;
		}

		/// <returns>Gets the size in bytes of one vertex</returns>
		size_t SizeBytes() const
		{
			return QueryNextOffset();
		}

		/// <returns>The number of added ElementTypes</returns>
		uint8_t Size() const
		{
			return (uint8_t)m_Elements.size();
		}

		Element& operator[](uint8_t i)
		{
			return m_Elements[i];
		}

		template<ElementType Type>
		size_t QueryOffset() const
		{
			size_t offset = 0;
			for (const Element& elem : m_Elements)
			{
				offset += elem.Offset();

				if (elem.Type() == Type)
					break;
			}

			return offset;
		}

		/// <summary>
		/// Gets the number of components in a specific type.
		/// The number of components in e.g. Position3D is (float x, y, z) 3
		/// </summary>
		/// <returns>The number of components in type</returns>
		constexpr uint8_t NumberOfComponentsInType(ElementType type) const
		{
			switch (type)
			{
			case Position2D:	return 2;
			case Position3D:	return 3;
			case Texture2D:		return 2;
			case Normal:		return 3;
			case Tangent:		return 3;
			case Bitangent:		return 3;
			case Float3Color:	return 3;
			case Float4Color:	return 4;
			case BGRAColor:		return 4;
			case Count:			return 1;
			}

			RAY_ASSERT(false, "[VertexLayout::NumberOfComponentsInType] ElementType (ID={0}) is invalid.", (uint8_t)type);
			return 0;
		}

	public:
		/// <summary>
		/// Specifies all the appended ElementTypes
		/// </summary>
		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				: m_Type(type), m_Offset(offset) {}

			size_t Offset() const
			{
				return m_Offset;
			}

			ElementType Type() const
			{
				return m_Type;
			}



		private:
			size_t m_Offset;
			ElementType m_Type;
		};

	private:
		size_t QueryNextOffset() const
		{
			size_t offset = 0;
			for (const Element& elem : m_Elements)
			{
				offset += elem.Offset();
			}

			if (m_Elements.size() > 0)
				offset += SizeOf(m_Elements.back().Type());

			return offset;
		}

	private:
		std::vector<Element> m_Elements;
	};


	// Basically a view into VertexData::m_Data
	class Vertex
	{
		friend class VertexData;
	public:
		/// <summary>
		/// Getter for the specified ElementType
		/// </summary>
		/// <returns>A view into the memory where the ElementType is stored</returns>
		template<VertexLayout::ElementType Type>
		auto& Attribute()
		{
			return *(typename VertexLayout::Map<Type>::SysType*)(m_VertexStart + m_Layout.QueryOffset<Type>());
		}

		/// <summary>
		/// Sets the specified ElementType. Expect unexpected results when trying to set a ElementType
		/// that was not added to the VertexLayout
		/// </summary>
		/// <param name="data">Is the data to set</param>
		/// <returns>Reference to the location where the data was set</returns>
		template<VertexLayout::ElementType Type>
		auto& Set(const typename VertexLayout::Map<Type>::SysType& data)
		{
			using SysType = typename VertexLayout::Map<Type>::SysType;

			SysType& type = *(SysType*)(m_VertexStart + m_Layout.QueryOffset<Type>());
			type = data;
			return type;
		}

	private:
		Vertex(char* vertexStart, const VertexLayout& layout)
			: m_VertexStart(vertexStart), m_Layout(layout) {}

	private:
		char* m_VertexStart;
		const VertexLayout& m_Layout;
	};

	// Const version
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v)
			: m_V(v) {}

		/// <summary>
		/// Getter for the specified ElementType
		/// </summary>
		/// <returns>A constant view into the memory where the ElementType is stored</returns>
		template<VertexLayout::ElementType Type>
		const auto& Attribute() const
		{
			return const_cast<Vertex&>(m_V).Attribute<Type>();
		}

	private:
		Vertex m_V;
	};


	// Vertices data storage
	class VertexData
	{
	public:
		VertexData(VertexLayout layout, size_t numVertices)
			: m_Layout(std::move(layout))
		{
			m_Data.resize(m_Layout.SizeBytes() * numVertices);
		}

		/// <returns>The size of all vertices in bytes</returns>
		size_t SizeBytes() const
		{
			return m_Data.size();
		}

		/// <returns>The number of vertices</returns>
		size_t Size() const
		{
			return m_Data.size() / m_Layout.SizeBytes();
		}

		uint8_t NumberOfElementTypes() const
		{
			return m_Layout.Size();
		}

		Vertex operator[](size_t i)
		{
			return { m_Data.data() + (m_Layout.SizeBytes() * i), m_Layout };
		}

		ConstVertex operator[](size_t i) const
		{
			return const_cast<VertexData&>(*this)[i];
		}

		VertexLayout& Layout() { return m_Layout; }
		const VertexLayout& Layout() const { return m_Layout; }

		void Swap(size_t i1, size_t i2)
		{
			char* data1 = m_Data.data() + i1;
			char* data2 = m_Data.data() + i2;

			std::swap_ranges(m_Data.begin() + i1, m_Data.begin() + i1 + m_Layout.SizeBytes(), m_Data.begin() + i2);
		}

		const char* Data() const
		{
			return m_Data.data();
		}

	private:
		VertexLayout m_Layout;

		// Holds all the vertex data.
		// Location anf offset for a specific Vertex is stored in the layout
		std::vector<char> m_Data;
	};

}