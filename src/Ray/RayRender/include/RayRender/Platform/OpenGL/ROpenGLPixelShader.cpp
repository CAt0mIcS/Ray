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
			std::string source = ReadShaderSource(filepath);
			const char* shaderSource = source.c_str();

			m_Shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_Shader, 1, &shaderSource, nullptr);
			glCompileShader(m_Shader);

			int success;
			glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);

			OnShaderCompilationFailed(success);
			CreateProgram();
		}
	}

	void OpenGLPixelShader::Bind()
	{
		BindProgram();
	}
}




