#pragma once

#include <stdint.h>


namespace At0::Ray
{
	struct Color
	{
		uint8_t r{};
		uint8_t g{};
		uint8_t b{};

		/**
		 * Takes the color range of 0 - 255 and normalizes it to 0.0f - 1.0f
		 */
		void Normalize(float* rOut, float* gOut, float* bOut)
		{
			*rOut = (float)r / 255.0f;
			*gOut = (float)g / 255.0f;
			*bOut = (float)b / 255.0f;
		}
	};
}  // namespace At0::Ray
