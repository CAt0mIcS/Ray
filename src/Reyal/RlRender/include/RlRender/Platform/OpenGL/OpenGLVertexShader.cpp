#include "rlrpch.h"
#include "OpenGLVertexShader.h"

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace At0::Reyal
{
	OpenGLVertexShader::OpenGLVertexShader(const std::string_view filepath, FileState state)
		: m_Name("")
	{
		RL_PROFILE_FUNCTION();

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);

		Compile(ReadFile(filepath).data());
	}
	
	OpenGLVertexShader::OpenGLVertexShader(const std::string_view name, const std::string_view vertexSrc)
		: m_Name(name)
	{
		RL_PROFILE_FUNCTION();
		Compile(vertexSrc.data());
	}
	
	void OpenGLVertexShader::Bind() const
	{
		RL_PROFILE_FUNCTION();

		glLinkProgram(s_RendererID);
		glValidateProgram(s_RendererID);
		//glDeleteShader(vs);
		//glDeleteShader(fs);
		glUseProgram(s_RendererID);
	}
	
	void OpenGLVertexShader::Unbind() const
	{
		RL_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLVertexShader::Compile(const char* source)
	{
		RL_PROFILE_FUNCTION();

		if (!s_RendererID)
			s_RendererID = glCreateProgram();

		uint32_t s = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(s, 1, &source, nullptr);
		glCompileShader(s);
		glAttachShader(s_RendererID, s);
	}
	
	std::string OpenGLVertexShader::ReadFile(const std::string_view filepath)
	{
		RL_PROFILE_FUNCTION();

		std::string result;
		std::ifstream reader(filepath.data(), std::ios::in | std::ios::binary);

		if (reader)
		{
			reader.seekg(0, std::ios::end);
			size_t size = reader.tellg();
			if (size != -1)
			{
				result.resize(size);
				reader.seekg(0, std::ios::beg);
				reader.read(&result[0], size);
			}
			else
			{
				RL_LOG_ERROR("[OpenGLVertexShader] Was unable to read the file {0}", filepath);
			}
		}
		else
		{
			RL_LOG_ERROR("[OpenGLVertexShader] Was unable to open the file {0}", filepath);
		}

		return result;
	}
}


