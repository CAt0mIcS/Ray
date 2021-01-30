#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class Color
	{
	public:
		enum PredefinedColorValues
		{
			Black = 0x000000ff,
			White = 0xffffffff
		};

	public:
		Color(PredefinedColorValues val)
		{
			r = (float)((uint8_t)(val >> 24)) / 255.0f;
			g = (float)((uint8_t)(val >> 16)) / 255.0f;
			b = (float)((uint8_t)(val >> 8)) / 255.0f;
			a = (float)((uint8_t)(val)) / 255.0f;
		}

	public:
		float r, g, b, a;
	};
}  // namespace At0::Ray
