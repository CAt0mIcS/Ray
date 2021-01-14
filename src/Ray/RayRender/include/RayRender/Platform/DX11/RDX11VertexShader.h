#pragma once

#include "../../RVertexShader.h"


namespace At0::Ray
{
	class RR_API DX11VertexShader : public VertexShader
	{
	public:
		DX11VertexShader(std::string_view filepath, bool compiled);
	};
}