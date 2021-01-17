#include "../../RRpch.h"

#include "ROpenGLPixelShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>

#include <glad/glad.h>


namespace At0::Ray
{
	OpenGLPixelShader::OpenGLPixelShader(std::string_view filepath, bool compiled)
	{
		if (compiled)
		{

		}
		else
		{
			m_PixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* pixelShaderSource = filepath.data();
			glShaderSource(m_PixelShader, 1, &pixelShaderSource, NULL);
			glCompileShader(m_PixelShader);

			int success;
			glGetShaderiv(m_PixelShader, GL_COMPILE_STATUS, &success);
			OnShaderCompilationFailed(success);
		}
	}

	void OpenGLPixelShader::Bind()
	{

	}
}




