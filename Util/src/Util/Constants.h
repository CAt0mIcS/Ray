#pragma once

#include "Util.h"

/** QUESTION:
*	Where should I put static global variables?
*	Should I use static global variables or inline or without any keywords
*/


namespace Constants
{
	inline constexpr Util::NColor g_DefaultLineColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultTextColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultLineButtonColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultWindowBackgroundColor{ 35.0f, 38.0f, 40.0f };
	inline constexpr Util::NColor g_DefaultNodeColor{ 15.0f, 17.0f, 19.0f };
	inline constexpr Util::NColor g_DefaultTextBoxColor{ 35.0f, 38.0f, 40.0f };
	inline constexpr Util::NColor g_DefaultCaretColor{ 255, 255, 255 };
	inline constexpr Util::NColor g_DefaultSelectionRectColor{ 80, 80, 80 };

	inline constexpr float g_SaveFileVersion = 1.0f;
}
