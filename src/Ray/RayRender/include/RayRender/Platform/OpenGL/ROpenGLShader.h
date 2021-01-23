#pragma once

#include "../../RShader.h"


namespace At0::Ray
{
	class RR_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() override;

		uint32_t GetShaderProgram() const { return m_Program; }

	private:
		std::string TranscompileShader(std::string_view filepath, bool vertexShader);

	private:
		uint32_t m_Program;
	};
}