#pragma once

#include "../../RBase.h"
#include "../../Core/RResource.h"
#include "../../Core/RMath.h"
#include "../../Graphics/Images/RTexture2DAtlas.h"

#include <unordered_map>


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
			Texture2DAtlas::Area area{};
		};

	public:
		Font(std::string_view filepath, uint32_t size, Type type);
		~Font();

		static Ref<Font> AcquireTTF(std::string_view filepath, uint32_t size);

		void Load(std::string_view filepath);
		const Glyph& GetGlyph(char c) const { return m_Glyphs.at(c); }
		Ref<Texture2DAtlas> GetSharedTextureAtlas() const { return m_TextureAtlas; }
		const Texture2DAtlas& GetTextureAtlas() const { return *m_TextureAtlas; }

	private:
		Ref<Texture2DAtlas> m_TextureAtlas;

		// Maps character to glyph
		std::unordered_map<char, Glyph> m_Glyphs;

		// Glyph size in pixels
		uint32_t m_Size{};
	};
}  // namespace At0::Ray
