#pragma once

#include "../../RPixelShader.h"
#include "ROpenGLShaderBase.h"


namespace At0::Ray
{
	class RR_API OpenGLPixelShader : public PixelShader, public OpenGLShaderBase
	{
	public:
		OpenGLPixelShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;
	};
}
