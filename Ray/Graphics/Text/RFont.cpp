#include "Rpch.h"
#include "RFont.h"

#define STB_TRUETYPE_IMPLEMENTATION

#include <stb/stb_truetype.h>
#include <msdf/msdf.h>


namespace At0::Ray
{
	static const std::string s_SupportedLetters =
		" \t\r\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890\"!`?'.,;:()[]{}<>|/"
		"@\\^$-%+=#_&~*";

	Font::Font(std::string_view filepath, uint32_t size, Type type) : m_Size(size)
	{
		Load(filepath);
	}

	Ref<Font> Font::AcquireTTF(std::string_view filepath, uint32_t size)
	{
		return Resources::Get().EmplaceIfNonExistent<Font>(
			filepath.data(), filepath, size, Type::TTF);
	}

	void Font::Load(std::string_view filepath)
	{
		// Load data
		std::ifstream reader(filepath, std::ios::ate | std::ios::binary);
		std::vector<uint8_t> bytes(reader.tellg());
		reader.seekg(std::ios::beg);
		reader.read((char*)bytes.data(), bytes.size());

		stbtt_fontinfo fontinfo;
		stbtt_InitFont(&fontinfo, bytes.data(), stbtt_GetFontOffsetForIndex(bytes.data(), 0));

		uint32_t layerCount = s_SupportedLetters.size();

		uint32_t arrayLayer = 0;
		for (auto c : s_SupportedLetters)
		{
			ex_metrics_t metrics = {};
			auto bitmap = ex_msdf_glyph(&fontinfo, c, m_Size, m_Size, &metrics, 1);
			if (bitmap)
			{
				// image->SetPixels(bitmap, arrayLayer);

				free(bitmap);
			}

			m_Glyphs.emplace_back(Glyph(metrics.left_bearing, metrics.advance,
				{ metrics.ix0, metrics.iy0 }, { metrics.ix1, metrics.iy1 }));
			m_Indices[c] = arrayLayer++;
		}
	}
}  // namespace At0::Ray
