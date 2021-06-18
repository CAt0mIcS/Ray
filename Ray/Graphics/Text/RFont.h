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
			Glyph() = default;

			Ref<Texture2D> texture{};
			Int2 size{};
			Int2 bearing{};
			uint32_t advance{};
		};

	public:
		Font(std::string_view filepath, uint32_t size, Type type);

		static Ref<Font> AcquireTTF(std::string_view filepath, uint32_t size);

		void Load(std::string_view filepath);

	private:
		Ref<Texture2D> m_Texture;

		// Maps character to glyph
		std::unordered_map<char, Glyph> m_Glyphs;

		// Glyph size in pixels
		uint32_t m_Size{};
	};
}  // namespace At0::Ray
