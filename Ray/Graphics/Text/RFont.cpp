#include "Rpch.h"
#include "RFont.h"

#include "Graphics/Images/RTexture2DAtlas.h"
#include "Graphics/Buffers/RBuffer.h"
#include "Utils/RException.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace At0::Ray
{
	static const std::string s_SupportedLetters =
		" \t\r\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890\"!`?'.,;:()[]{}<>|/"
		"@\\^$-%+=#_&~*";
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
		return Resources::Get().EmplaceIfNonExistent<Font>(
			filepath.data(), filepath, size, Type::TTF);
	}

	static void CreateTextureFromBitmap(FT_GlyphSlot glyphSlot, Texture2DAtlas& atlas)
	{
		if (glyphSlot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY ||
			glyphSlot->bitmap.num_grays != 256)
			ThrowRuntime("[Font] Unsupported pixel mode");

		uint32_t width = glyphSlot->bitmap.width;
		uint32_t height = glyphSlot->bitmap.rows;
		uint32_t bufferSize = width * height * 4;

		if (bufferSize == 0)
			return;

		std::vector<uint8_t> buffer(bufferSize);

		uint8_t* src = glyphSlot->bitmap.buffer;
		uint8_t* startOfLine = src;
		int dst = 0;

		for (int y = 0; y < height; ++y)
		{
			src = startOfLine;
			for (int x = 0; x < width; ++x)
			{
				auto value = *src;
				src++;

				buffer[dst++] = 0xff;
				buffer[dst++] = 0xff;
				buffer[dst++] = 0xff;
				buffer[dst++] = value;
			}
			startOfLine += glyphSlot->bitmap.pitch;
		}

		atlas.Emplace({ width, height }, buffer.data());
	}

	void Font::Load(std::string_view filepath)
	{
		FT_Face face;
		if (FT_Error error = FT_New_Face(s_FTLibrary, filepath.data(), 0, &face))
			ThrowRuntime("[Font] Failed to create new face (Error code {0}).", error);

		// FT_Set_Pixel_Sizes(face, 0, m_Size);
		FT_Set_Char_Size(face, 0, 1500 * 64,  // char height in 1/64th of points
			96,								  // horizontal device resolution
			96								  // vertical device resolution
		);

		UInt2 atlasSize{};
		for (char supportedChar : s_SupportedLetters)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(face, supportedChar);
			if (FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
				ThrowRuntime("[Font] Failed to load glyph for character '{0}' (Error code {1})",
					supportedChar, error);

			if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
				if (FT_Error error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
					ThrowRuntime(
						"[Font] Failed to render glyph for character '{0}' (Error code {1})",
						supportedChar, error);

			m_Glyphs[supportedChar] = Glyph({ face->glyph->bitmap.width, face->glyph->bitmap.rows },
				{ face->glyph->bitmap_left, face->glyph->bitmap_top }, face->glyph->advance.x);
			atlasSize.x += face->glyph->bitmap.width;
			atlasSize.y += face->glyph->bitmap.rows;
		}

		m_TextureAtlas = MakeRef<Texture2DAtlas>(atlasSize / UInt2(4) /*UInt2{ 13500, 13500 }*/,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

		for (char supportedChar : s_SupportedLetters)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(face, supportedChar);
			if (FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
				ThrowRuntime("[Font] Failed to load glyph for character '{0}' (Error code {1})",
					supportedChar, error);

			if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
				if (FT_Error error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
					ThrowRuntime(
						"[Font] Failed to render glyph for character '{0}' (Error code {1})",
						supportedChar, error);

			CreateTextureFromBitmap(face->glyph, *m_TextureAtlas);
		}
	}
}  // namespace At0::Ray
