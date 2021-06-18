#include "Rpch.h"
#include "RFont.h"

#include "Graphics/Images/RTexture2D.h"
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

	Ref<Font> Font::AcquireTTF(std::string_view filepath, uint32_t size)
	{
		return Resources::Get().EmplaceIfNonExistent<Font>(
			filepath.data(), filepath, size, Type::TTF);
	}

	static Ref<Texture2D> CreateTextureFromBitmap(FT_GlyphSlot glyphSlot)
	{
		if (glyphSlot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY ||
			glyphSlot->bitmap.num_grays != 256)
			ThrowRuntime("[Font] Unsupported pixel mode");

		uint32_t width = glyphSlot->bitmap.width;
		uint32_t height = glyphSlot->bitmap.rows;
		uint32_t bufferSize = width * height * 4;

		if (bufferSize == 0)
			ThrowRuntime("[Font] Invalid buffer size 0");

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

		Ref<Texture2D> texture = Texture2D::Acquire({ width, height }, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT);

		Buffer vulkanBuffer(buffer.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			buffer.data());

		texture->CopyFromBuffer(vulkanBuffer);
		return texture;
	}

	void Font::Load(std::string_view filepath)
	{
		FT_Face face;
		if (FT_Error error = FT_New_Face(s_FTLibrary, filepath.data(), 0, &face))
			ThrowRuntime("[Font] Failed to create new face (Error code {0}).", error);

		FT_Set_Pixel_Sizes(face, 0, m_Size);

		for (char supportedChar : s_SupportedLetters)
		{
			// if (FT_Error error = FT_Load_Char(face, supportedChar, FT_LOAD_RENDER))
			//	ThrowRuntime("[Font] Failed to load character '{0}' (Error code {1}).",
			//		supportedChar, error);

			FT_Load_Glyph(face, FT_Get_Char_Index(face, supportedChar), FT_LOAD_DEFAULT);
			if (FT_Error error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
				ThrowRuntime("[Font] Failed to render character '{0}' (Error code {1})",
					supportedChar, error);

			Ref<Texture2D> texture = CreateTextureFromBitmap(face->glyph);
			m_Glyphs[supportedChar] =
				Glyph(texture, { face->glyph->bitmap.width, face->glyph->bitmap.rows },
					{ face->glyph->bitmap_left, face->glyph->bitmap_top }, face->glyph->advance.x);
		}
	}
}  // namespace At0::Ray
