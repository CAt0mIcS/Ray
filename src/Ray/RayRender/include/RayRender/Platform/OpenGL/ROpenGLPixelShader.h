#pragma once

#include "../../RPixelShader.h"
#include "ROpenGLShaderBase.h"


namespace At0::Ray
{
	class RR_API OpenGLPixelShader : public PixelShader, OpenGLShaderBase
	{
	public:
		OpenGLPixelShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;

	private:
		uint32_t m_PixelShader;
	};
}
