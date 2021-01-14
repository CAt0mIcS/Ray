#pragma once

#include "../../RPixelShader.h"


namespace At0::Ray
{
	class RR_API DX11PixelShader : public PixelShader
	{
	public:
		DX11PixelShader(std::string_view filepath, bool compiled);
	};
}