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
			m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
			const char* shaderSource = filepath.data();
			glShaderSource(m_VertexShader, 1, &shaderSource, NULL);
			glCompileShader(m_VertexShader);

			int success;
			glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);

			OnShaderCompilationFailed(success);
		}
	}

	void OpenGLVertexShader::Bind()
	{
		BindProgram();
	}
}




