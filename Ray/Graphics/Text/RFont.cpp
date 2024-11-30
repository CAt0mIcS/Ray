#include "RFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace At0::Ray
{
	const std::vector<uint8_t> Font::SupportedCharacters = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
		42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
		65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
		88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
		109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126 };

	static FT_Library s_FTLibrary = nullptr;

	Font::Font(std::string_view filepath, uint32_t size, Type type) : m_Size(size)
	{
		if (!s_FTLibrary)
		{
			if (FT_Error error = FT_Init_FreeType(&s_FTLibrary))
				ThrowRuntime("[Font] Failed to initialize FreeType (Error code {0}).", error);
		}

		Load(filepath);
	}

	Font::~Font() {}

	Ref<Font> Font::AcquireTTF(std::string_view filepath, uint32_t size)
	{
		return ResourceManager::Get().EmplaceOrGet<Font>(
			filepath.data(), filepath, size, Type::TTF);
	}

	static Ref<Texture> CreateTextureFromBitmap(FT_GlyphSlot glyphSlot)
	{
		if (glyphSlot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY ||
			glyphSlot->bitmap.num_grays != 256)
			ThrowRuntime("[Font] Unsupported pixel mode");

		uint32_t width = glyphSlot->bitmap.width;
		uint32_t height = glyphSlot->bitmap.rows;
		uint32_t bufferSize = width * height;

		if (bufferSize == 0)
			return nullptr;

		auto texture =
			Texture::Builder()
				.SetExtent({ width, height })
				.SetFormat(VK_FORMAT_R8_UNORM)
				.SetImageUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
				.SetMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				.SetTextureSampler(TextureSampler::Builder()
									   .SetMinFilter(VK_FILTER_LINEAR)
									   .SetMagFilter(VK_FILTER_LINEAR)
									   .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
									   .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
									   .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
									   .BuildScoped())
				.SetData(glyphSlot->bitmap.buffer, bufferSize)
				.Build();
		texture->TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		return texture;
	}

	void Font::Load(std::string_view filepath)
	{
		FT_Face face;
		if (FT_Error error = FT_New_Face(s_FTLibrary, filepath.data(), 0, &face))
			ThrowRuntime("[Font] Failed to create new face (Error code {0}).", error);

		FT_Set_Pixel_Sizes(face, 0, m_Size);

		// FT_Set_Char_Size(face, 0, m_Size * 64,	// char height in 1/64th of points
		//	96,									// horizontal device resolution
		//	96									// vertical device resolution
		//);

		for (uint8_t c : SupportedCharacters)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
			if (FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
				ThrowRuntime(
					"[Font] Failed to load glyph for character '{0}' (Error code {1})", c, error);

			if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
				if (FT_Error error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
					ThrowRuntime(
						"[Font] Failed to render glyph for character '{0}' (Error code {1})", c,
						error);

			// Store glyph and convert advance from 1/64th a pixel to 1 pixel
			Glyph& glyph = m_Glyphs[c] = Glyph(
				{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
				{ face->glyph->bitmap_left, face->glyph->bitmap_top }, face->glyph->advance.x >> 6);

			if (Ref<Texture> texture = CreateTextureFromBitmap(face->glyph))
				glyph.texture = std::move(texture);
			else
				Log::Error("[Font] Texture creation for character '{0}' in font \"{1}\" failed", c,
					filepath);
		}

		FT_Done_Face(face);
	}

	bool Font::IsLoaded(uint8_t c) const
	{
		return m_Glyphs.find(c) != m_Glyphs.end() && m_Glyphs.at(c).texture;
	}
}  // namespace At0::Ray
