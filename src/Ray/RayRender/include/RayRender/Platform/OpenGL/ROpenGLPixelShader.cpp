#include "../../RRpch.h"

#include "ROpenGLPixelShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>

#include <glad/glad.h>
#include <ShaderConductor/ShaderConductor.hpp>
namespace SC = ShaderConductor;

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
			SC::Compiler::SourceDesc sd{};

			sd.fileName = filepath.data();
			sd.stage = SC::ShaderStage::PixelShader;
			sd.source = source.c_str();

			SC::Compiler::Options options{};
			SC::Compiler::TargetDesc td{};
			td.language = SC::ShadingLanguage::Glsl;

			SC::Compiler::ResultDesc rd = SC::Compiler::Compile(sd, options, td);
			if (rd.hasError)
				RAY_THROW_RUNTIME("[OpenGLPixelShader] Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());

			// Strip any weird characters at the end
			source = (char*)rd.target.Data();
			source.resize(rd.target.Size());
			const char* shaderSource = source.c_str();

			Log::Debug("[OpenGLPixelShader] Converted PixelShader {0} to:\n{1}", filepath, shaderSource);

			m_Shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_Shader, 1, &shaderSource, nullptr);
			glCompileShader(m_Shader);

			int success;
			glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);

			OnPixelShaderCompilationFailed(success);
			CreateProgram();
		}
	}

	void OpenGLPixelShader::Bind()
	{
		BindProgram();
	}
}




