#include "../../RRpch.h"
#include "ROpenGLShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>

#include <glad/glad.h>
#include <ShaderConductor/ShaderConductor.hpp>
namespace SC = ShaderConductor;

namespace At0::Ray
{
	static std::string ReadShaderSource(std::string_view filepath)
	{
		std::ifstream reader(filepath.data());
		std::streampos fileSize = reader.tellg();
		reader.seekg(0, std::ios::end);
		fileSize = reader.tellg() - fileSize;
		reader.seekg(0, std::ios::beg);

		std::string source;
		source.resize(fileSize);
		reader.read(source.data(), fileSize);

		return source;
	}


	OpenGLShader::OpenGLShader(std::string_view vertexFilepath, std::string_view pixelFilepath)
		: m_Program(0)
	{
		uint32_t vertexShader;
		uint32_t pixelShader;

		// --------------------------------------------------------------
		// Vertex Shader
		{
			std::string shaderSource = TranscompileShader(vertexFilepath, true);
			const char* source = shaderSource.c_str();

			vertexShader = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(vertexShader, 1, &source, nullptr);
			glCompileShader(vertexShader);

			int success;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				Log::Critical("[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
				RAY_ASSERT(false, "[OpenGLVertexShader] Vertex Shader Compilation failed: {0}", infoLog);
			}
		}

		// --------------------------------------------------------------
		// Pixel Shader
		{
			std::string shaderSource = TranscompileShader(pixelFilepath, false);
			const char* source = shaderSource.c_str();

			pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(pixelShader, 1, &source, nullptr);
			glCompileShader(pixelShader);

			int success;
			glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
				Log::Critical("[OpenGLVertexShader] Pixel Shader Compilation failed: {0}", infoLog);
				RAY_ASSERT(false, "[OpenGLVertexShader] Pixel Shader Compilation failed: {0}", infoLog);
			}
		}

		// --------------------------------------------------------------
		// Create Program
		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertexShader);
		glAttachShader(m_Program, pixelShader);
		glLinkProgram(m_Program);

		int success;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			RAY_THROW_RUNTIME("[OpenGLSHader] Shader Linkage failed: {0}", infoLog);
		}

		glUseProgram(m_Program);
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_Program);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	std::string OpenGLShader::TranscompileShader(std::string_view filepath, bool vertexShader)
	{
		std::string source = ReadShaderSource(filepath);
		SC::Compiler::SourceDesc sd{};

		sd.fileName = filepath.data();
		if (vertexShader)
			sd.stage = SC::ShaderStage::VertexShader;
		else
			sd.stage = SC::ShaderStage::PixelShader;
		sd.source = source.c_str();

		SC::Compiler::Options options{};
		SC::Compiler::TargetDesc td{};
		td.language = SC::ShadingLanguage::Glsl;

		SC::Compiler::ResultDesc rd = SC::Compiler::Compile(sd, options, td);
		if (rd.hasError)
			if (vertexShader)
				RAY_THROW_RUNTIME("[OpenGLShader] Vertex Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());
			else
				RAY_THROW_RUNTIME("[OpenGLShader] Pixel Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());


		// Strip any weird characters at the end
		source = (char*)rd.target.Data();
		source.resize(rd.target.Size());
		const char* shaderSource = source.c_str();

		if (vertexShader)
			Log::Debug("[OpenGLShader] Converted VertexShader {0} to:\n{1}", filepath, shaderSource);
		else
			Log::Debug("[OpenGLShader] Converted PixelShader {0} to:\n{1}", filepath, shaderSource);

		return source;
	}
}