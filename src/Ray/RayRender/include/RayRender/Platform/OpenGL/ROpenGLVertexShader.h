#pragma once

#include "../../RVertexShader.h"
#include "ROpenGLShaderBase.h"


namespace At0::Ray
{
	class RR_API OpenGLVertexShader : public VertexShader, public OpenGLShaderBase
	{
		friend class OpenGLInputLayout;
	public:
		OpenGLVertexShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;
	};
}
