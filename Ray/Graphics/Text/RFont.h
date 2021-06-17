#pragma once

#include "../../RBase.h"
#include "../../Core/RResource.h"
#include "../../Core/RMath.h"

#include <unordered_map>


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT Font : public Resource
	{
	public:
		enum class Type
		{
			TTF
		};

		struct Glyph
		{
			Glyph(int32_t leftBearing, int32_t advance, Int2 i0, Int2 i1)
				: leftBearing(leftBearing), advance(advance), i0(std::move(i0)), i1(std::move(i1))
			{
			}

			int32_t leftBearing;
			int32_t advance;
			Int2 i0, i1;
		};

	public:
		Font(std::string_view filepath, uint32_t size, Type type);

		static Ref<Font> AcquireTTF(std::string_view filepath, uint32_t size);

		void Load(std::string_view filepath);

	private:
		Ref<Texture2D> m_Texture;
		std::vector<Glyph> m_Glyphs;

		// Maps character to glyph index in m_Glyphs
		std::unordered_map<char, uint32_t> m_Indices;

		// Glyph size in pixels
		uint32_t m_Size{};
	};
}  // namespace At0::Ray
