#include "Rpch.h"
#include "RShader.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	class ShaderIncluder : public glslang::TShader::Includer
	{
	public:
		IncludeResult* includeLocal(
			const char* headerName, const char* includerName, size_t inclusionDepth) override
		{
			auto dir = std::filesystem::path(includerName).parent_path();
			std::optional<std::string> fileContents = ReadFile(dir / headerName);

			if (!fileContents)
			{
				Log::Error(
					"[ShaderIncluder] Include file {0} could not be loaded.", dir / headerName);
				return nullptr;
			}

			char* contents = new char[fileContents->size()];
			memcpy(contents, fileContents->c_str(), fileContents->size());
			return new IncludeResult(headerName, contents, fileContents->size(), contents);
		}

		IncludeResult* includeSystem(
			const char* headerName, const char* includerName, size_t inclusionDepth) override
		{
			std::optional<std::string> fileContents = ReadFile(headerName);

			if (!fileContents)
			{
				Log::Error("[ShaderIncluder] Include file {0} could not be loaded.", headerName);
				return nullptr;
			}

			char* contents = new char[fileContents->size()];
			memcpy(contents, fileContents->c_str(), fileContents->size());
			return new IncludeResult(headerName, contents, fileContents->size(), contents);
		}

		void releaseInclude(IncludeResult* result) override
		{
			if (result)
			{
				delete[](char*) result->userData;
				delete result;
			}
		}
	};


	Shader::Shader()
	{
		static bool glslangInitialized = false;
		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}
	}

	VkFormat Shader::GlTypToVkFormat(int32_t type)
	{
		switch (type)
		{
		case 0x1406:  // GL_FLOAT
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50:  // GL_FLOAT_VEC2
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51:  // GL_FLOAT_VEC3
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52:  // GL_FLOAT_VEC4
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case 0x1404:  // GL_INT
			return VK_FORMAT_R32_SINT;
		case 0x8B53:  // GL_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8B54:  // GL_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8B55:  // GL_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		case 0x1405:  // GL_UNSIGNED_INT
			return VK_FORMAT_R32_SINT;
		case 0x8DC6:  // GL_UNSIGNED_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8DC7:  // GL_UNSIGNED_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8DC8:  // GL_UNSIGNED_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		default: return VK_FORMAT_UNDEFINED;
		}
	}

	TBuiltInResource GetResources()
	{
		TBuiltInResource resources = {};
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.limits.nonInductiveForLoops = true;
		resources.limits.whileLoops = true;
		resources.limits.doWhileLoops = true;
		resources.limits.generalUniformIndexing = true;
		resources.limits.generalAttributeMatrixVectorIndexing = true;
		resources.limits.generalVaryingIndexing = true;
		resources.limits.generalSamplerIndexing = true;
		resources.limits.generalVariableIndexing = true;
		resources.limits.generalConstantMatrixVectorIndexing = true;
		return resources;
	}

	VkShaderModule Shader::CreateShaderModule(const std::filesystem::path& moduleName,
		std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag)
	{
		// Convert GLSL to SPIRV
		EShLanguage language = GetEShLanguage(moduleFlag);

		glslang::TProgram program;
		glslang::TShader shader(language);
		TBuiltInResource resource = GetResources();

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = EShMessages(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#ifndef NDEBUG
		messages = EShMessages(messages | EShMsgDebugInfo);
#endif

		std::string shaderName = moduleName.string();
		const char* shaderNameCstr = shaderName.c_str();
		const char* shaderSource = moduleCode.data();
		shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderNameCstr, 1);
		shader.setPreamble(preamble.data());

		glslang::EShTargetClientVersion defaultVersion = glslang::EShTargetVulkan_1_1;
		shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
		shader.setEnvClient(glslang::EShClientVulkan, defaultVersion);
		shader.setEnvTarget(
			glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);	// RAY_TODO: Check if supported

		ShaderIncluder includer;

		std::string outStr;

		if (!shader.preprocess(
				&resource, defaultVersion, ENoProfile, false, false, messages, &outStr, includer))
		{
			Log::Error("[Shader] Shader preproccessing failed: {0}.", shader.getInfoLog());
			Log::Error("[Shader] {0}.", shader.getInfoDebugLog());
		}

		if (!shader.parse(&resource, defaultVersion, true, messages, includer))
		{
			Log::Error("[Shader] Shader preproccessing failed: {0}.", shader.getInfoLog());
			Log::Error("[Shader] {0}.", shader.getInfoDebugLog());
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
			Log::Error("[Shader] Failed to link shader.");

		program.buildReflection();

		for (uint32_t dim = 0; dim < 3; ++dim)
		{
		}

		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; --i)
		{
		}

		for (int32_t i = 0; i < program.getNumLiveUniformVariables(); ++i)
		{
		}

		for (int32_t i = 0; i < program.getNumLiveAttributes(); ++i)
		{
		}

		glslang::SpvOptions spvOptions;
#ifndef NDEBUG
		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;
#else
		spvOptions.generateDebugInfo = false;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = true;
#endif

		spv::SpvBuildLogger logger;
		std::vector<uint32_t> spirvCode;
		GlslangToSpv(
			*program.getIntermediate((EShLanguage)language), spirvCode, &logger, &spvOptions);

		VkShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = spirvCode.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = spirvCode.data();

		VkShaderModule shaderModule;
		RAY_VK_THROW_FAILED(vkCreateShaderModule(Graphics::Get().GetDevice(),
								&shaderModuleCreateInfo, nullptr, &shaderModule),
			"[Shader] Failed to create shader module.");

		return shaderModule;
	}

	VkShaderStageFlagBits Shader::GetShaderStage(const std::filesystem::path& filepath)
	{
		std::string fileExt = filepath.extension().string();
		std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);

		if (fileExt == ".comp")
			return VK_SHADER_STAGE_COMPUTE_BIT;
		if (fileExt == ".vert")
			return VK_SHADER_STAGE_VERTEX_BIT;
		if (fileExt == ".tesc")
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (fileExt == ".tese")
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if (fileExt == ".geom")
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		if (fileExt == ".frag")
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		return VK_SHADER_STAGE_ALL;
	}

	EShLanguage Shader::GetEShLanguage(VkShaderStageFlags stageFlags)
	{
		switch (stageFlags)
		{
		case VK_SHADER_STAGE_VERTEX_BIT: return EShLangVertex;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return EShLangFragment;
		case VK_SHADER_STAGE_COMPUTE_BIT: return EShLangCompute;
		case VK_SHADER_STAGE_GEOMETRY_BIT: return EShLangGeometry;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return EShLangTessEvaluation;
		}

		RAY_THROW_RUNTIME("[Shader] Stage flag {0} is incompatible.", stageFlags);
		return EShLangAnyHit;
	}

	void Shader::CreateReflection() {}
}  // namespace At0::Ray
