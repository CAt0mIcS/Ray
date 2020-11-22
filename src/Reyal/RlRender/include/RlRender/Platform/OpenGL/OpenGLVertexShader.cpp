#include "rlrpch.h"
#include "OpenGLVertexShader.h"

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <glad/glad.h>


namespace At0::Reyal
{
	OpenGLVertexShader::OpenGLVertexShader(const std::string_view filepath, FileState state)
		: m_RendererID(0), m_Name("")
	{
		RL_PROFILE_FUNCTION();

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);

		Compile(ReadFile(filepath));
	}
	
	OpenGLVertexShader::OpenGLVertexShader(const std::string_view name, const std::string_view vertexSrc)
		: m_RendererID(0), m_Name(name)
	{
		RL_PROFILE_FUNCTION();

	}
	
	void OpenGLVertexShader::Bind() const
	{
		RL_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}
	
	void OpenGLVertexShader::Unbind() const
	{
		RL_PROFILE_FUNCTION();
		glUseProgram(0);
	}
	
	void OpenGLVertexShader::Compile(const std::string_view source)
	{
		RL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		GLuint shader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* shaderSrc = source.data();
		glShaderSource(shader, 1, &shaderSrc, 0);

		glCompileShader(shader);
		GLint isCompiled = 0;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			RL_LOG_ERROR("[OpenGLVertexShader] {0}", infoLog.data());
			RL_ASSERT(false, "Shader Compilation Failed!");
		}
		else
		{
			glAttachShader(program, shader);
		}

		m_RendererID = program;
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(shader);

			RL_LOG_ERROR("[OpenGLVertexShader] {0}", infoLog.data());
			RL_ASSERT(false, "Shader Linking Failed!");
			return;
		}

		glDetachShader(program, shader);
		glDeleteShader(shader);
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


