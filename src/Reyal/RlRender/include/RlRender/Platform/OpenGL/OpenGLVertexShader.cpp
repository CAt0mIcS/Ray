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
		: m_Name(""), m_ShaderBound(false)
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

		GLint isLinked = 0;
		glGetProgramiv(s_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(s_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(s_RendererID, maxLength, &maxLength, &infoLog[0]);

				RL_LOG_ERROR("[OpenGLVertexShader] {0}", infoLog.data());
				RL_ASSERT(false, "Shader Linking Failed!");
			}
			glDeleteProgram(s_RendererID);

			return;
		}

		glUseProgram(s_RendererID);
	}
	
	void OpenGLVertexShader::Unbind() const
	{
		RL_PROFILE_FUNCTION();
		glUseProgram(0);
	}
	
	static void GLFWErrorCallback(int error, const char* description)
	{
		RL_LOG_CRITICAL("[OpenGLWindow] GLFW Error ({0}): {1}", error, description);
		RL_LOG_FLUSH();
	}

	void OpenGLVertexShader::Compile(const char* source)
	{
		RL_PROFILE_FUNCTION();

		glfwSetErrorCallback(GLFWErrorCallback);

		GLuint program;
		if (!s_RendererID)
			program = glCreateProgram();
		else
			program = s_RendererID;

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &source, nullptr);

		glCompileShader(vertexShader);
		GLint isCompiled = 0;

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
				RL_LOG_ERROR("[OpenGLVertexShader] {0}", infoLog.data());
				RL_ASSERT(false, "Shader Compilation Failed!");
			}
			glDeleteShader(vertexShader);
		}
		else
		{
			glAttachShader(program, vertexShader);
		}

		s_RendererID = program;
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


