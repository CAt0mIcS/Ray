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

	void Font::Load(std::string_view filepath) {}
}  // namespace At0::Ray
