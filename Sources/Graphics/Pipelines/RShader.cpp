﻿#include "Rpch.h"
#include "RShader.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Utils/RString.h"

#include "Core/RVertex.h"

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


	Shader::Shader() : m_VertexLayout(MakeScope<VertexLayout>())
	{
		static bool glslangInitialized = false;
		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}
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
		default: return VK_FORMAT_UNDEFINED;
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

		RAY_ASSERT(false, "[SizeOfGlType] Failed to find glType {0}", glType);
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

		RAY_THROW_RUNTIME("[Shader] Stage flag {0} is incompatible.", stageFlags);
		return EShLangAnyHit;
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

	void Shader::CreateReflection() {}

	std::vector<VkVertexInputAttributeDescription>
		Shader::GetVertexInputAttributeDescriptions() const
	{
		return m_VertexLayout->GetVertexInputAttributeDescriptions();
	}

	std::vector<VkVertexInputBindingDescription> Shader::GetVertexInputBindingDescriptions(
		uint32_t binding) const
	{
		return m_VertexLayout->GetVertexInputBindingDescriptions(binding);
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

		RAY_ASSERT(false, "[Shader] Vulkan shader stage {0} is invalid.", (uint32_t)stageFlags);
		return Stage::Vertex;
	}

	std::optional<Shader::UniformBlocks> Shader::GetUniformBlocks(Shader::Stage stage) const
	{
		if (auto& it = m_ShaderData.find(stage); it != m_ShaderData.end())
			return it->second.uniformBlocks;
		return std::nullopt;
	}

	std::optional<Shader::Uniforms> Shader::GetUniforms(Shader::Stage stage) const
	{
		if (auto& it = m_ShaderData.find(stage); it != m_ShaderData.end())
			return it->second.uniforms;
		return std::nullopt;
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

	void Shader::LoadUniform(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniform = program.getUniform(i);

		Shader::Uniforms::UniformData data{};
		data.binding = uniform.getBinding();
		data.size = uniform.size;

		// Sampler is not stored in the global uniform buffer and thus does not have to be taken
		// into account when calculating the next offset in said buffer.
		// (RAY_TODO: Add more types that aren't stored in a uniform buffer)
		if (uniform.getType()->getBasicType() != glslang::TBasicType::EbtSampler)
			data.offset = CalculateNextOffset(SizeOfGlType(uniform.glDefineType));

		// -1 means that it's in a block
		if (data.binding == -1)
		{
			// Insert into the already existing uniform block
			auto strings = String::Split(uniform.name, '.');
			m_ShaderData[stageFlag].uniformBlocks.m_UniformBlocks[strings[0]].uniforms.Emplace(
				strings[1], data);
		}
		else
		{
			m_ShaderData[stageFlag].uniforms.Emplace(uniform.name, data);
		}
	}

	void Shader::LoadUniformBlock(
		const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniformBlock = program.getUniformBlock(i);

		UniformBlocks::UniformBlockData data{};
		data.binding = uniformBlock.getBinding();
		data.size = uniformBlock.size;

		m_ShaderData[stageFlag].uniformBlocks.Emplace(uniformBlock.name, data);
	}

	void Shader::LoadAttribute(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& attribute = program.getPipeInput(i);

		const glslang::TQualifier& qualifier = attribute.getType()->getQualifier();

		Attributes::AttributeData data;
		data.location = qualifier.layoutLocation;
		data.binding = attribute.getBinding();
		data.size = attribute.size;
		data.format = GlTypeToVkFormat(attribute.glDefineType);

		m_ShaderData[stageFlag].attributes.Emplace(attribute.name, data);

		m_VertexLayout->Append(data.format);
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

	uint32_t Shader::CalculateNextOffset(uint32_t uniformSize)
	{
		static uint32_t currentOffset = 0;
		uint32_t prevOffset = currentOffset;

		currentOffset += uniformSize;

		return prevOffset;
	}


	// ------------------------------------------------------------
	// Attributes
	void Shader::Attributes::Emplace(std::string_view attributeName, const AttributeData& data)
	{
		m_Attributes[attributeName.data()] = data;
	}


	// ------------------------------------------------------------
	// Uniforms
	void Shader::Uniforms::Emplace(std::string_view uniformName, const UniformData& data)
	{
		m_Uniforms[uniformName.data()] = data;
	}

	std::optional<Shader::Uniforms::UniformData> Shader::Uniforms::Get(
		std::string_view uniformName) const
	{
		if (auto& it = m_Uniforms.find(uniformName.data()); it != m_Uniforms.end())
			return it->second;
		return std::nullopt;
	}


	// ------------------------------------------------------------
	// Uniform Blocks
	void Shader::UniformBlocks::Emplace(
		std::string_view uniformBlockName, const UniformBlockData& data)
	{
		m_UniformBlocks[uniformBlockName.data()] = data;
	}

	std::optional<Shader::UniformBlocks::UniformBlockData> Shader::UniformBlocks::Get(
		std::string_view uniformBlockName) const
	{
		if (auto& it = m_UniformBlocks.find(uniformBlockName.data()); it != m_UniformBlocks.end())
			return it->second;
		return std::nullopt;
	}
}  // namespace At0::Ray