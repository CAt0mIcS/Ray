#pragma once

#include "../../RBase.h"
#include "../../Core/RResource.h"
#include "../../Core/RMath.h"
#include "../../Graphics/Images/RTexture2DAtlas.h"

#include <unordered_map>
#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Font : public Resource
	{
	public:
		enum class Type
		{
			TTF
		};

		struct Glyph
		{
			Glyph(Int2 size, Int2 bearing, uint32_t advance)
				: size(size), bearing(bearing), advance(advance)
			{
			}
			Glyph() = default;

			Int2 size{};
			Int2 bearing{};
			uint32_t advance{};
			Texture2DDAtlas::Area area{};
		};

	public:
		Font(std::string_view filepath, uint32_t size, Type type);
		~Font();

		static Ref<Font> AcquireTTF(std::string_view filepath, uint32_t size);

		void Load(std::string_view filepath);
		const Glyph& GetGlyph(char c) const { return m_Glyphs.at(c); }
		Ref<Texture2DDAtlas> GetSharedTexture2DDAtlas() const { return m_Texture2DDAtlas; }
		const Texture2DDAtlas& GetTexture2DDAtlas() const { return *m_Texture2DDAtlas; }

	private:
		Ref<Texture2DDAtlas> m_Texture2DDAtlas;

		// Maps character to glyph
		std::unordered_map<char, Glyph> m_Glyphs;

		// Glyph size in pixels
		uint32_t m_Size{};
	};
}  // namespace At0::Ray
