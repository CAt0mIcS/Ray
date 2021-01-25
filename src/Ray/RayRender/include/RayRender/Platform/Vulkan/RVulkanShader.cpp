#include "../../RRpch.h"
#include "RVulkanShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>

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


	// std::string OpenGLShader::TranscompileShader(std::string_view filepath, bool vertexShader)
	// {
	// 	std::string source = ReadShaderSource(filepath);
	// 	SC::Compiler::SourceDesc sd{};

	// 	sd.fileName = filepath.data();
	// 	if (vertexShader)
	// 		sd.stage = SC::ShaderStage::VertexShader;
	// 	else
	// 		sd.stage = SC::ShaderStage::PixelShader;
	// 	sd.source = source.c_str();

	// 	SC::Compiler::Options options{};
	// 	SC::Compiler::TargetDesc td{};
	// 	td.language = SC::ShadingLanguage::Glsl;

	// 	SC::Compiler::ResultDesc rd = SC::Compiler::Compile(sd, options, td);
	// 	if (rd.hasError)
	// 		if (vertexShader)
	// 			RAY_THROW_RUNTIME("[OpenGLShader] Vertex Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());
	// 		else
	// 			RAY_THROW_RUNTIME("[OpenGLShader] Pixel Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());


	// 	// Strip any weird characters at the end
	// 	source = (char*)rd.target.Data();
	// 	source.resize(rd.target.Size());
	// 	const char* shaderSource = source.c_str();

	// 	if (vertexShader)
	// 		Log::Debug("[OpenGLShader] Converted VertexShader {0} to:\n{1}", filepath, shaderSource);
	// 	else
	// 		Log::Debug("[OpenGLShader] Converted PixelShader {0} to:\n{1}", filepath, shaderSource);

	// 	return source;
	// }
}