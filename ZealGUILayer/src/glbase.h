#pragma once

#ifdef ZEAL_GUI_LAYER_BUILD
#define ZGL_API __declspec(dllexport)
#else
#define ZGL_API __declspec(dllimport)
#endif
