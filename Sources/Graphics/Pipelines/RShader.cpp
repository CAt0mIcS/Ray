#include "Rpch.h"
#include "RShader.h"

#include "Core/RDynamicVertex.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Utils/RString.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Buffers/RUniformBuffer.h"


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
					"[ShaderIncluder] Include file {0} could not be loaded", dir / headerName);
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
				Log::Error("[ShaderIncluder] Include file {0} could not be loaded", headerName);
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


	Shader::Shader(std::vector<std::string> shaders)
	{
		static bool glslangInitialized = false;
		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}

		for (const std::string& shader : shaders)
		{
			VkShaderStageFlags vkShaderStage = GetShaderStage(shader);
			Shader::Stage shaderStage = ToShaderStage(vkShaderStage);

			m_Shaders[shaderStage] = shader;
			m_ShaderModules[shaderStage] = CreateShaderModule("", vkShaderStage);
		}
		CreateReflection();
	}

	Shader::~Shader() {}

	VkFormat Shader::GlTypeToVkFormat(int32_t type)
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
		default: Log::Warn("[Shader] Undefined GL type {0}", type); return VK_FORMAT_UNDEFINED;
		}
	}

	uint32_t SizeOfGlType(int32_t glType)
	{
		switch (glType)
		{
		case 0x1406:  // GL_FLOAT
		case 0x1404:  // GL_INT
		case 0x1405:  // GL_UNSIGNED_INT
			return 4;
		case 0x8B50:  // GL_FLOAT_VEC2
		case 0x8B53:  // GL_INT_VEC2
			return 2 * 4;
		case 0x8B51:  // GL_FLOAT_VEC3
		case 0x8B54:  // GL_INT_VEC3
			return 3 * 4;
		case 0x8DC6:  // GL_UNSIGNED_INT_VEC2
			return 2 * 4;
		case 0x8DC7:  // GL_UNSIGNED_INT_VEC3
			return 3 * 4;
		case 0x8B52:  // GL_FLOAT_VEC4
		case 0x8B55:  // GL_INT_VEC4
		case 0x8DC8:  // GL_UNSIGNED_INT_VEC4
			return 4 * 4;
		case 0x8B5C:  // GL_MAT4
			return 64;
		}

		RAY_ASSERT(false, "[Shader] Failed to find size of GL type {0}", glType);
		return 0;
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

	EShLanguage GetEShLanguage(VkShaderStageFlags stageFlags)
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

		RAY_THROW_RUNTIME("[Shader] Stage flag {0} is incompatible", stageFlags);
		return EShLangAnyHit;
	}

	VkShaderModule Shader::CreateShaderModule(
		std::string_view preamble, VkShaderStageFlags moduleFlag)
	{
		std::string moduleName = m_Shaders[ToShaderStage(moduleFlag)];
		std::optional<std::string> moduleCode = ReadFile(moduleName);
		RAY_MEXPECTS(moduleCode, "[Shader] Failed to read file \"{0}\"", moduleName);

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

		const char* shaderNameCstr = moduleName.c_str();
		const char* shaderSource = (*moduleCode).data();
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
			Log::Error("[Shader] Shader preproccessing failed: {0}", shader.getInfoLog());
			Log::Error("[Shader] {0}", shader.getInfoDebugLog());
		}

		if (!shader.parse(&resource, defaultVersion, true, messages, includer))
		{
			Log::Error("[Shader] Shader preproccessing failed: {0}", shader.getInfoLog());
			Log::Error("[Shader] {0}", shader.getInfoDebugLog());
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
			Log::Error("[Shader] Failed to link shader");

		program.buildReflection();

		for (uint32_t dim = 0; dim < 3; ++dim)
		{
		}

		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; --i)
		{
			LoadUniformBlock(program, ToShaderStage(moduleFlag), i);
		}

		for (int32_t i = program.getNumLiveUniformVariables() - 1; i >= 0; --i)
		{
			LoadUniform(program, ToShaderStage(moduleFlag), i);
		}

		for (int32_t i = 0; i < program.getNumLiveAttributes(); ++i)
		{
			LoadAttribute(program, ToShaderStage(moduleFlag), i);
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
			"[Shader] Failed to create shader module");

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

	uint32_t Shader::SizeOf(VkFormat format)
	{
		switch (format)
		{
		case VK_FORMAT_UNDEFINED: return 0;
		case VK_FORMAT_R4G4_UNORM_PACK8: return 1;
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
		case VK_FORMAT_R5G6B5_UNORM_PACK16:
		case VK_FORMAT_B5G6R5_UNORM_PACK16:
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;
		case VK_FORMAT_R8_UNORM:
		case VK_FORMAT_R8_SNORM:
		case VK_FORMAT_R8_USCALED:
		case VK_FORMAT_R8_SSCALED:
		case VK_FORMAT_R8_UINT:
		case VK_FORMAT_R8_SINT:
		case VK_FORMAT_R8_SRGB: return 1;
		case VK_FORMAT_R8G8_UNORM:
		case VK_FORMAT_R8G8_SNORM:
		case VK_FORMAT_R8G8_USCALED:
		case VK_FORMAT_R8G8_SSCALED:
		case VK_FORMAT_R8G8_UINT:
		case VK_FORMAT_R8G8_SINT:
		case VK_FORMAT_R8G8_SRGB: return 2;
		case VK_FORMAT_R8G8B8_UNORM:
		case VK_FORMAT_R8G8B8_SNORM:
		case VK_FORMAT_R8G8B8_USCALED:
		case VK_FORMAT_R8G8B8_SSCALED:
		case VK_FORMAT_R8G8B8_UINT:
		case VK_FORMAT_R8G8B8_SINT:
		case VK_FORMAT_R8G8B8_SRGB:
		case VK_FORMAT_B8G8R8_UNORM:
		case VK_FORMAT_B8G8R8_SNORM:
		case VK_FORMAT_B8G8R8_USCALED:
		case VK_FORMAT_B8G8R8_SSCALED:
		case VK_FORMAT_B8G8R8_UINT:
		case VK_FORMAT_B8G8R8_SINT:
		case VK_FORMAT_B8G8R8_SRGB: return 3;
		case VK_FORMAT_R8G8B8A8_UNORM:
		case VK_FORMAT_R8G8B8A8_SNORM:
		case VK_FORMAT_R8G8B8A8_USCALED:
		case VK_FORMAT_R8G8B8A8_SSCALED:
		case VK_FORMAT_R8G8B8A8_UINT:
		case VK_FORMAT_R8G8B8A8_SINT:
		case VK_FORMAT_R8G8B8A8_SRGB:
		case VK_FORMAT_B8G8R8A8_UNORM:
		case VK_FORMAT_B8G8R8A8_SNORM:
		case VK_FORMAT_B8G8R8A8_USCALED:
		case VK_FORMAT_B8G8R8A8_SSCALED:
		case VK_FORMAT_B8G8R8A8_UINT:
		case VK_FORMAT_B8G8R8A8_SINT:
		case VK_FORMAT_B8G8R8A8_SRGB: return 4;
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_UINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_UINT_PACK32:
		case VK_FORMAT_A2R10G10B10_SINT_PACK32:
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_UINT_PACK32:
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: return 4;
		case VK_FORMAT_R16_UNORM:
		case VK_FORMAT_R16_SNORM:
		case VK_FORMAT_R16_USCALED:
		case VK_FORMAT_R16_SSCALED:
		case VK_FORMAT_R16_UINT:
		case VK_FORMAT_R16_SINT:
		case VK_FORMAT_R16_SFLOAT: return 2;
		case VK_FORMAT_R16G16_UNORM:
		case VK_FORMAT_R16G16_SNORM:
		case VK_FORMAT_R16G16_USCALED:
		case VK_FORMAT_R16G16_SSCALED:
		case VK_FORMAT_R16G16_UINT:
		case VK_FORMAT_R16G16_SINT:
		case VK_FORMAT_R16G16_SFLOAT: return 4;
		case VK_FORMAT_R16G16B16_UNORM:
		case VK_FORMAT_R16G16B16_SNORM:
		case VK_FORMAT_R16G16B16_USCALED:
		case VK_FORMAT_R16G16B16_SSCALED:
		case VK_FORMAT_R16G16B16_UINT:
		case VK_FORMAT_R16G16B16_SINT:
		case VK_FORMAT_R16G16B16_SFLOAT: return 6;
		case VK_FORMAT_R16G16B16A16_UNORM:
		case VK_FORMAT_R16G16B16A16_SNORM:
		case VK_FORMAT_R16G16B16A16_USCALED:
		case VK_FORMAT_R16G16B16A16_SSCALED:
		case VK_FORMAT_R16G16B16A16_UINT:
		case VK_FORMAT_R16G16B16A16_SINT:
		case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;
		case VK_FORMAT_R32_UINT:
		case VK_FORMAT_R32_SINT:
		case VK_FORMAT_R32_SFLOAT: return 4;
		case VK_FORMAT_R32G32_UINT:
		case VK_FORMAT_R32G32_SINT:
		case VK_FORMAT_R32G32_SFLOAT: return 8;
		case VK_FORMAT_R32G32B32_UINT:
		case VK_FORMAT_R32G32B32_SINT:
		case VK_FORMAT_R32G32B32_SFLOAT: return 12;
		case VK_FORMAT_R32G32B32A32_UINT:
		case VK_FORMAT_R32G32B32A32_SINT:
		case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;
		case VK_FORMAT_R64_UINT:
		case VK_FORMAT_R64_SINT:
		case VK_FORMAT_R64_SFLOAT: return 8;
		case VK_FORMAT_R64G64_UINT:
		case VK_FORMAT_R64G64_SINT:
		case VK_FORMAT_R64G64_SFLOAT: return 16;
		case VK_FORMAT_R64G64B64_UINT:
		case VK_FORMAT_R64G64B64_SINT:
		case VK_FORMAT_R64G64B64_SFLOAT: return 24;
		case VK_FORMAT_R64G64B64A64_UINT:
		case VK_FORMAT_R64G64B64A64_SINT:
		case VK_FORMAT_R64G64B64A64_SFLOAT: return 32;
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;
		case VK_FORMAT_D16_UNORM: return 2;
		case VK_FORMAT_X8_D24_UNORM_PACK32:
		case VK_FORMAT_D32_SFLOAT: return 4;
		case VK_FORMAT_S8_UINT: return 1;
		case VK_FORMAT_D16_UNORM_S8_UINT: return 3;
		case VK_FORMAT_D24_UNORM_S8_UINT: return 4;
		case VK_FORMAT_D32_SFLOAT_S8_UINT: return 5;
		}

		RAY_ASSERT(false, "[Shader] VkFormat {0} unknown", (uint32_t)format);
		return 0;
	}

	void Shader::CreateReflection()
	{
		std::unordered_map<VkDescriptorType, uint32_t> descriptorPoolCounts;

		for (auto& [shaderStage, shaderData] : m_ShaderData)
		{
			// Sort attributes by location and add their format to the vertex layout
			std::sort(shaderData.attributes.begin(), shaderData.attributes.end(),
				[](const Attributes::AttributeData& l, const Attributes::AttributeData& r) {
					return l.location < r.location;
				});

			if (shaderStage == Shader::Stage::Vertex)
			{
				// for (const auto& attribData : shaderData.attributes)
				//	m_VertexLayout->Append(attribData.format);
			}

			for (auto& uniformBlock : shaderData.uniformBlocks)
			{
				VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

				switch (uniformBlock.type)
				{
				case Shader::UniformBlocks::Type::Uniform:
				{
					descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

					m_DescriptorSetLayoutBindings[uniformBlock.set].emplace_back(
						UniformBuffer::GetDescriptorSetLayout((uint32_t)uniformBlock.binding,
							descriptorType, ToVkShaderStage(shaderStage), 1));
					break;
				}
				}

				IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
			}

			for (const auto& uniformData : shaderData.uniforms)
			{
				VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
				switch (uniformData.glType)
				{
				case 0x8B5E:  // GL_SAMPLER_2D
				case 0x904D:  // GL_IMAGE_2D
				case 0x8DC1:  // GL_TEXTURE_2D_ARRAY
				case 0x9108:  // GL_SAMPLER_2D_MULTISAMPLE
				case 0x9055:  // GL_IMAGE_2D_MULTISAMPLE
				{
					descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					VkDescriptorSetLayoutBinding samplerLayoutBinding{};
					samplerLayoutBinding.binding = uniformData.binding;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.descriptorType = descriptorType;
					samplerLayoutBinding.pImmutableSamplers = nullptr;
					samplerLayoutBinding.stageFlags = ToVkShaderStage(shaderStage);

					descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					m_DescriptorSetLayoutBindings[uniformData.set].emplace_back(
						samplerLayoutBinding);
					break;
				}
				}

				IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
			}
		}


		for (const auto& [type, descriptorCount] : descriptorPoolCounts)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = type;
			poolSize.descriptorCount = descriptorCount;
			m_DescriptorPoolSizes.emplace_back(poolSize);
		}

		// RAY_TODO: This is a AMD workaround that works on NVidia too...
		// We don't know the total usages of descriptor types by the pipeline.
		m_DescriptorPoolSizes.resize(6);
		m_DescriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		m_DescriptorPoolSizes[0].descriptorCount = 4096;
		m_DescriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_DescriptorPoolSizes[1].descriptorCount = 2048;
		m_DescriptorPoolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		m_DescriptorPoolSizes[2].descriptorCount = 2048;
		m_DescriptorPoolSizes[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		m_DescriptorPoolSizes[3].descriptorCount = 2048;
		m_DescriptorPoolSizes[4].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
		m_DescriptorPoolSizes[4].descriptorCount = 2048;
		m_DescriptorPoolSizes[5].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		m_DescriptorPoolSizes[5].descriptorCount = 2048;
	}

	bool Shader::HasUniform(std::string_view name, Shader::Stage stage) const
	{
		if (auto data = m_ShaderData.find(stage); data != m_ShaderData.end())
		{
			if (data->second.uniformBlocks.HasUniform(name) ||
				data->second.uniformBlocks.HasUniformBlock(name))
				return true;

			if (data->second.uniforms.HasUniform(name))
				return true;
		}

		return false;
	}

	Shader::Stage Shader::ToShaderStage(VkShaderStageFlags stageFlags)
	{
		switch (stageFlags)
		{
		case VK_SHADER_STAGE_VERTEX_BIT: return Stage::Vertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return Stage::TesselationControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return Stage::TesselationEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT: return Stage::Geometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return Stage::Fragment;
		case VK_SHADER_STAGE_COMPUTE_BIT: return Stage::Compute;
		}

		RAY_ASSERT(false, "[Shader] Vulkan shader stage {0} is invalid", (uint32_t)stageFlags);
		return Stage::Vertex;
	}

	VkShaderStageFlagBits Shader::ToVkShaderStage(Shader::Stage stageFlags)
	{
		switch (stageFlags)
		{
		case Stage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case Stage::TesselationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case Stage::TesselationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case Stage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case Stage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case Stage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		RAY_ASSERT(false, "[Shader] Vulkan shader stage {0} is invalid", (uint32_t)stageFlags);
		return VK_SHADER_STAGE_VERTEX_BIT;
	}

	const Shader::UniformBlocks* Shader::GetUniformBlocks(Shader::Stage stage) const
	{
		try
		{
			return &m_ShaderData.at(stage).uniformBlocks;
		}
		catch (std::out_of_range&)
		{
			return nullptr;
		}
	}

	const Shader::Uniforms* Shader::GetUniforms(Shader::Stage stage) const
	{
		try
		{
			return &m_ShaderData.at(stage).uniforms;
		}
		catch (std::out_of_range&)
		{
			return nullptr;
		}
	}

	const Shader::Attributes* Shader::GetAttributes(Shader::Stage stage) const
	{
		try
		{
			return &m_ShaderData.at(stage).attributes;
		}
		catch (std::out_of_range&)
		{
			return nullptr;
		}
	}

	std::vector<Shader::Stage> Shader::GetLiveShaderStages() const
	{
		std::vector<Shader::Stage> stages;
		stages.reserve(m_ShaderData.size());

		for (auto& it : m_ShaderData)
		{
			stages.emplace_back(it.first);
		}

		return stages;
	}

	std::vector<VkPushConstantRange> Shader::GetPushConstantRanges() const
	{
		std::vector<VkPushConstantRange> pushConstantRanges;

		uint32_t currentOffset = 0;
		for (const auto& [shaderStage, shaderData] : m_ShaderData)
		{
			for (const auto& uniformBlockData : shaderData.uniformBlocks)
			{
				if (uniformBlockData.type != UniformBlocks::Type::Push)
					continue;

				VkPushConstantRange pushConstantRange{};
				pushConstantRange.stageFlags = ToVkShaderStage(shaderStage);
				pushConstantRange.offset = currentOffset;
				pushConstantRange.size = uniformBlockData.size;
				pushConstantRanges.emplace_back(pushConstantRange);

				currentOffset += uniformBlockData.size;
			}
		}

		return pushConstantRanges;
	}

	std::vector<VkVertexInputBindingDescription> Shader::GetVertexInputBindings(
		uint32_t binding) const
	{
		return DynamicVertex{ *this }.GetVertexInputBindings(binding);
	}

	std::vector<VkVertexInputAttributeDescription> Shader::GetVertexInputAttributes(
		uint32_t binding) const
	{
		return DynamicVertex{ *this }.GetVertexInputAttributes(binding);
	}

	std::string Shader::GetUID(const std::vector<std::string>& shaders)
	{
		std::ostringstream oss;
		oss << "Shader";
		for (const std::string& str : shaders)
			oss << "#" << str;
		return oss.str();
	}

	void Shader::LoadUniform(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniform = program.getUniform(i);

		Shader::Uniforms::UniformData data{};
		data.binding = uniform.getBinding();
		data.size = uniform.size;
		data.glType = uniform.glDefineType;
		data.offset = uniform.offset;
		data.set = uniform.getType()->getQualifier().layoutSet;
		data.uniformName = uniform.name;

		// -1 means that it's in a block
		if (data.binding == -1)
		{
			// Insert into the already existing uniform block
			auto strings = String::Split(uniform.name, '.');
			data.uniformName = strings[1];

			for (UniformBlocks::UniformBlockData& uBlockData :
				m_ShaderData[stageFlag].uniformBlocks)
			{
				if (uBlockData.uniformBlockName == strings[0])
					uBlockData.uniforms.Emplace(data);
			}
		}
		else
		{
			m_ShaderData[stageFlag].uniforms.Emplace(data);
		}
	}

	void Shader::LoadUniformBlock(
		const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniformBlock = program.getUniformBlock(i);

		UniformBlocks::UniformBlockData data{};
		data.binding = uniformBlock.getBinding();
		data.size = uniformBlock.size;
		data.set = uniformBlock.getType()->getQualifier().layoutSet;
		data.uniformBlockName = uniformBlock.name;

		if (uniformBlock.getType()->getQualifier().storage == glslang::EvqUniform)
			data.type = Shader::UniformBlocks::Type::Uniform;
		// if (uniformBlock.getType()->getQualifier().storage == glslang::EvqBuffer)
		//	data.type = Shader::UniformBlocks::Type::Storage;
		if (uniformBlock.getType()->getQualifier().layoutPushConstant)
			data.type = Shader::UniformBlocks::Type::Push;

		m_ShaderData[stageFlag].uniformBlocks.Emplace(data);
	}

	void Shader::LoadAttribute(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& attribute = program.getPipeInput(i);

		const glslang::TQualifier& qualifier = attribute.getType()->getQualifier();

		Attributes::AttributeData data;
		data.location = qualifier.layoutLocation;
		data.binding = attribute.getBinding();
		data.format = GlTypeToVkFormat(attribute.glDefineType);
		data.size = Shader::SizeOf(data.format);
		data.attributeName = attribute.name;

		m_ShaderData[stageFlag].attributes.Emplace(data);
	}

	int32_t Shader::ComputeSize(const glslang::TType* ttype)
	{
		int32_t components = 0;

		if (ttype->getBasicType() == glslang::EbtStruct ||
			ttype->getBasicType() == glslang::EbtBlock)
		{
			for (const auto& tl : *ttype->getStruct())
				components += ComputeSize(tl.type);
		}
		else if (ttype->getMatrixCols() != 0)
			components = ttype->getMatrixCols() * ttype->getMatrixRows();
		else
			components = ttype->getVectorSize();

		if (ttype->getArraySizes())
		{
			int32_t arraySize = 1;

			for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d)
			{
				// This only makes sense in paths that have a known array size.
				if (auto dimSize = ttype->getArraySizes()->getDimSize(d);
					dimSize != glslang::UnsizedArraySize)
					arraySize *= dimSize;
			}

			components *= arraySize;
		}

		return sizeof(float) * components;
	}

	void Shader::IncrementDescriptorPool(
		std::unordered_map<VkDescriptorType, uint32_t>& descriptorPoolCounts, VkDescriptorType type)
	{
		if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM)
			return;

		if (auto it = descriptorPoolCounts.find(type); it != descriptorPoolCounts.end())
			it->second++;
		else
			descriptorPoolCounts.emplace(type, 1);
	}

	// ------------------------------------------------------------
	// Uniforms
	const Shader::Uniforms::UniformData* Shader::Uniforms::Get(std::string_view uniformName) const
	{
		for (const UniformData& dt : m_Uniforms)
		{
			if (dt.uniformName == uniformName)
				return &dt;
		}
		return nullptr;
	}

	bool Shader::Uniforms::HasUniform(std::string_view name) const
	{
		for (auto& uniform : m_Uniforms)
		{
			if (uniform.uniformName == name)
				return true;
		}
		return false;
	}


	// ------------------------------------------------------------
	// Uniform Blocks
	const Shader::UniformBlocks::UniformBlockData* Shader::UniformBlocks::Get(
		std::string_view uniformBlockName) const
	{
		for (const UniformBlockData& dt : m_UniformBlocks)
		{
			if (dt.uniformBlockName == uniformBlockName)
				return &dt;
		}
		return nullptr;
	}

	bool Shader::UniformBlocks::HasUniformBlock(std::string_view name) const
	{
		for (auto& uBlock : m_UniformBlocks)
		{
			if (uBlock.uniformBlockName == name)
				return true;
		}
		return false;
	}

	bool Shader::UniformBlocks::HasUniform(std::string_view name) const
	{
		for (auto& uBlock : m_UniformBlocks)
		{
			if (uBlock.uniforms.HasUniform(name))
				return true;
		}
		return false;
	}
}  // namespace At0::Ray
