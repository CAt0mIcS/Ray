#pragma once

#include "../../RPixelShader.h"


namespace At0::Ray
{
	class RR_API OpenGLPixelShader : public PixelShader
	{
	public:
		OpenGLPixelShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;
	};
}
