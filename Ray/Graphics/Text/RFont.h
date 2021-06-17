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
			Glyph(Ref<Texture2D> texture, Int2 size, Int2 bearing, uint32_t advance)
				: texture(std::move(texture)), size(size), bearing(bearing), advance(advance)
			{
			}

			Ref<Texture2D> texture;
			Int2 size;
			Int2 bearing;
			uint32_t advance;
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
