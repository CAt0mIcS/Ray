#include "../../RRpch.h"

#include "ROpenGLVertexShader.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RAssert.h>

#include <glad/glad.h>


namespace At0::Ray
{
	OpenGLVertexShader::OpenGLVertexShader(std::string_view filepath, bool compiled)
	{
		if (compiled)
		{

		}
		else
		{
			std::string source = ReadShaderSource(filepath);
			const char* shaderSource = source.c_str();

			m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_VertexShader, 1, &shaderSource, nullptr);
			glCompileShader(m_VertexShader);

			int success;
			glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);

			OnShaderCompilationFailed(success);
			CreateProgram();
		}
	}

	void OpenGLVertexShader::Bind()
	{
		BindProgram();
	}
}




