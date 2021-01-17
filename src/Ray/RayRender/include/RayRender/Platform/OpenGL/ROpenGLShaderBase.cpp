#include "../../RRpch.h"
#include "ROpenGLShaderBase.h"

#include <RayDebug/RAssert.h>

#include <glad/glad.h>


namespace At0::Ray
{
	void OpenGLShaderBase::AttachVertexShader(uint32_t shaderID)
	{
		m_VertexShader = shaderID;

		if (m_PixelShader != (uint32_t)-1)
		{
			CreateProgram();
		}
	}

	void OpenGLShaderBase::AttachPixelShader(uint32_t shaderID)
	{
		m_PixelShader = shaderID;

		if (m_VertexShader != (uint32_t)-1)
		{
			CreateProgram();
		}
	}

	void OpenGLShaderBase::BindProgram()
	{
		glUseProgram(m_ShaderProgram);
	}

	void OpenGLShaderBase::OnShaderCompilationFailed(int success)
	{
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(m_VertexShader, 512, NULL, infoLog);
			Log::Critical("[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
			RAY_ASSERT(false, "[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
		}
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
		glAttachShader(m_ShaderProgram, m_VertexShader);
		glAttachShader(m_ShaderProgram, m_PixelShader);

		glLinkProgram(m_ShaderProgram);

		int success;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
		OnShaderLinkageFailed(success);

		glUseProgram(m_ShaderProgram);

		glDeleteShader(m_VertexShader);
		glDeleteShader(m_PixelShader);
	}
}