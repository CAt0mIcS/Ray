#include "RRpch.h"
#include "RShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11Shader.h"
#include "Platform/OpenGL/ROpenGLShader.h"
#include "Platform/Vulkan/RVulkanShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include <ShaderConductor/ShaderConductor.hpp>
namespace SC = ShaderConductor;


namespace At0::Ray
{
	Scope<Shader> Shader::Create(std::string_view vertexFilepath, std::string_view pixelFilepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[Shader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11Shader>(vertexFilepath, pixelFilepath);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLShader>(vertexFilepath, pixelFilepath);
		case RendererAPI::Vulkan:	return MakeScope<VulkanShader>(vertexFilepath, pixelFilepath);
			break;
		}

		return nullptr;
	}
	
	std::string Shader::TranscompileShader(std::string_view filepath, ShaderConductor::ShaderStage stage, ShaderConductor::ShadingLanguage outputLanguage)
	{
		std::string source = ReadShaderSource(filepath);
		SC::Compiler::SourceDesc sd{};

		sd.fileName = filepath.data();
		sd.stage = stage;
		sd.source = source.c_str();

		SC::Compiler::Options options{};
		SC::Compiler::TargetDesc td{};
		td.language = outputLanguage;

		SC::Compiler::ResultDesc rd = SC::Compiler::Compile(sd, options, td);
		if (rd.hasError)
			RAY_THROW_RUNTIME("[OpenGLShader] Shader Transcompilation failed with message: {0}", (char*)rd.errorWarningMsg.Data());

		// Strip any weird characters at the end
		source = (char*)rd.target.Data();
		source.resize(rd.target.Size());

		Log::Debug("[OpenGLShader] Converted Shader {0} to:\n{1}", filepath, source);

		return source;
	}

	std::string Shader::ReadShaderSource(std::string_view filepath)
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
}
