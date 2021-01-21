#include "../../RRpch.h"
#include "ROpenGLShaderBase.h"

#include <RayDebug/RAssert.h>

#include <glad/glad.h>


namespace At0::Ray
{
	void OpenGLShaderBase::BindProgram()
	{
		glUseProgram(m_ShaderProgram);
	}

	void OpenGLShaderBase::OnVertexShaderCompilationFailed(int success)
	{
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
			Log::Critical("[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
			RAY_ASSERT(false, "[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
		}
	}

	void OpenGLShaderBase::OnPixelShaderCompilationFailed(int success)
	{
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
			Log::Critical("[OpenGLVertexShader] Pixel Shader Compilation failed: {0}", infoLog);
			RAY_ASSERT(false, "[OpenGLVertexShader] Pixel Shader Compilation failed: {0}", infoLog);
		}
	}

	std::string OpenGLShaderBase::ReadShaderSource(std::string_view filepath)
	{
		std::fstream reader(filepath.data());

		std::string line;
		std::string source;
		while (std::getline(reader, line))
		{
			source += line;
			source += '\n';
		}
		return source;
	}

	void OpenGLShaderBase::OnShaderLinkageFailed(int success)
	{
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
			Log::Critical("[OpenGLVertexShader] Vertex Shader Linkage failed: {0}", infoLog);
			RAY_ASSERT(false, "[OpenGLVertexShader] Vertex Shader Linkage failed: {0}", infoLog);
		}
	}

	void OpenGLShaderBase::CreateProgram()
	{
		m_ShaderProgram = glCreateProgram();

		glAttachShader(m_ShaderProgram, m_Shader);
		glLinkProgram(m_ShaderProgram);

		int success;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
		OnShaderLinkageFailed(success);

		glUseProgram(m_ShaderProgram);

		glDeleteShader(m_Shader);
	}
}