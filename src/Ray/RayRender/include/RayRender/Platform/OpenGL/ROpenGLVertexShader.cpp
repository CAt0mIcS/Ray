#include "../../RRpch.h"

#include "ROpenGLVertexShader.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RAssert.h>

#include <glad/glad.h>

#include <ShaderConductor/ShaderConductor.hpp>
namespace SC = ShaderConductor;


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
			SC::Compiler::SourceDesc sd{};

			sd.fileName = filepath.data();
			sd.stage = SC::ShaderStage::VertexShader;
			sd.source = source.c_str();

			SC::Compiler::Options options{};
			SC::Compiler::TargetDesc td{};
			td.language = SC::ShadingLanguage::Glsl;

			SC::Compiler::ResultDesc rd = SC::Compiler::Compile(sd, options, td);
			if (rd.hasError)
				RAY_THROW_RUNTIME("[OpenGLVertexShader] Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());

			// Strip any weird characters at the end
			source = (char*)rd.target.Data();
			source.resize(rd.target.Size());
			const char* shaderSource = source.c_str();

			Log::Debug("[OpenGLVertexShader] Converted VertexShader {0} to:\n{1}", filepath, shaderSource);

			m_Shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_Shader, 1, &shaderSource, nullptr);
			glCompileShader(m_Shader);

			int success;
			glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);

			OnVertexShaderCompilationFailed(success);
			CreateProgram();
		}
	}

	void OpenGLVertexShader::Bind()
	{
		BindProgram();
	}
}




