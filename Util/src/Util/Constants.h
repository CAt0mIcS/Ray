#pragma once

#include "Util.h"

/** QUESTION:
*	Where should I put static global variables?
*	Should I use static global variables or inline or without any keywords
*	Should I add g_ in a namespace named "Constants"?
*/


namespace Constants
{
	inline constexpr Util::NColor g_DefaultLineColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultTextColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultLineButtonColor{ 160.0f, 160.0f, 160.0f };
	inline constexpr Util::NColor g_DefaultWindowBackgroundColor{ 35.0f, 38.0f, 40.0f };
	inline constexpr Util::NColor g_DefaultNodeColor{ 15.0f, 17.0f, 19.0f };
	inline constexpr Util::NColor g_DefaultTextBoxColor{ 35.0f, 38.0f, 40.0f };
	inline constexpr Util::NColor g_DefaultCaretColor{ 255.0f, 255.0f, 255.0f };
	inline constexpr Util::NColor g_DefaultSelectionRectColor{ 80.0f, 80.0f, 80.0f };
	
	/**
	* QUESTION:
	*	Should I put this in the SceneTab class?
	*/
	inline constexpr Util::NColor g_DefaultActiveSceneTabColor{ 0.0f, 122.0f, 204.0f };
	inline constexpr Util::NColor g_DefaultInactiveSceneTabColor{ 45.0f, 45.0f, 48.0f };
	inline constexpr Util::NColor g_DefaultTabBorderColor{ 15.0f, 15.0f, 15.0f };
	inline constexpr Util::NSize  g_DefaultTabSize{ 100.0f, 25.0f };

	inline constexpr float g_SaveFileVersion = 1.0f;
}
