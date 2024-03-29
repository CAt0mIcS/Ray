﻿#include "RGlslCompiler.h"

#include "RShader.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/ROStringStream.h"

#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/MachineIndependent/gl_types.h>


namespace At0::Ray
{
#pragma region Helper

	static std::optional<std::string> ReadFile(const std::filesystem::path& filepath)
	{
		std::ifstream reader(filepath, std::ios::binary | std::ios::ate);

		size_t filesize = (size_t)reader.tellg();
		if (filesize == std::numeric_limits<uint64_t>::max())
			return std::nullopt;

		std::string file;
		file.resize(filesize);
		reader.seekg(std::ios::beg);

		reader.read(file.data(), filesize);

		return file;
	}

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

	static VkFormat GlTypeToVkFormat(int32_t type)
	{
		switch (type)
		{
		case GL_FLOAT: return VK_FORMAT_R32_SFLOAT;
		case GL_FLOAT_VEC2: return VK_FORMAT_R32G32_SFLOAT;
		case GL_FLOAT_VEC3: return VK_FORMAT_R32G32B32_SFLOAT;
		case GL_FLOAT_VEC4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case GL_INT: return VK_FORMAT_R32_SINT;
		case GL_INT_VEC2: return VK_FORMAT_R32G32_SINT;
		case GL_INT_VEC3: return VK_FORMAT_R32G32B32_SINT;
		case GL_INT_VEC4: return VK_FORMAT_R32G32B32A32_SINT;
		case GL_UNSIGNED_INT: return VK_FORMAT_R32_SINT;
		case GL_UNSIGNED_INT_VEC2: return VK_FORMAT_R32G32_SINT;
		case GL_UNSIGNED_INT_VEC3: return VK_FORMAT_R32G32B32_SINT;
		case GL_UNSIGNED_INT_VEC4: return VK_FORMAT_R32G32B32A32_SINT;
		default:
			Log::Warn("[GlslCompiler] Undefined GL type {0}", type);
			return VK_FORMAT_UNDEFINED;
		}
	}

	static UniformType GlTypeToUniformType(int32_t glType)
	{
		switch (glType)
		{
		case GL_SAMPLER_2D:
		case GL_IMAGE_2D:
		case GL_SAMPLER_2D_ARRAY:
		case GL_SAMPLER_2D_MULTISAMPLE:
		case GL_IMAGE_2D_MULTISAMPLE: return UniformType::CombinedImageSampler;
		}
		return UniformType::None;
	}

	static ShaderDataType GlTypeToDataType(int32_t glType)
	{
		switch (glType)
		{
		case GL_FLOAT: return ShaderDataType::Float;
		case GL_FLOAT_VEC2: return ShaderDataType::Vec2;
		case GL_FLOAT_VEC3: return ShaderDataType::Vec3;
		case GL_FLOAT_VEC4: return ShaderDataType::Vec4;

		case GL_INT: return ShaderDataType::Int;
		case GL_INT_VEC2: return ShaderDataType::IVec2;
		case GL_INT_VEC3: return ShaderDataType::IVec3;
		case GL_INT_VEC4: return ShaderDataType::IVec4;

		case GL_UNSIGNED_INT: return ShaderDataType::UInt;
		case GL_UNSIGNED_INT_VEC2: return ShaderDataType::UVec2;
		case GL_UNSIGNED_INT_VEC3: return ShaderDataType::UVec3;
		case GL_UNSIGNED_INT_VEC4: return ShaderDataType::UVec4;

		case GL_BOOL: return ShaderDataType::Bool;
		case GL_BOOL_VEC2: return ShaderDataType::BVec2;
		case GL_BOOL_VEC3: return ShaderDataType::BVec3;
		case GL_BOOL_VEC4: return ShaderDataType::BVec4;

		case GL_DOUBLE: return ShaderDataType::Double;
		case GL_DOUBLE_VEC2: return ShaderDataType::DVec2;
		case GL_DOUBLE_VEC3: return ShaderDataType::DVec3;
		case GL_DOUBLE_VEC4: return ShaderDataType::DVec4;

		case GL_FLOAT_MAT2: return ShaderDataType::Mat2;
		case GL_FLOAT_MAT2x3: return ShaderDataType::Mat2x3;
		case GL_FLOAT_MAT2x4: return ShaderDataType::Mat2x4;

		case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
		case GL_FLOAT_MAT3x2: return ShaderDataType::Mat3x2;
		case GL_FLOAT_MAT3x4: return ShaderDataType::Mat3x4;

		case GL_FLOAT_MAT4: return ShaderDataType::Mat4;
		case GL_FLOAT_MAT4x2: return ShaderDataType::Mat4x2;
		case GL_FLOAT_MAT4x3: return ShaderDataType::Mat4x3;

		case GL_DOUBLE_MAT2: return ShaderDataType::DMat2;
		case GL_DOUBLE_MAT2x3: return ShaderDataType::DMat2x3;
		case GL_DOUBLE_MAT2x4: return ShaderDataType::DMat2x4;

		case GL_DOUBLE_MAT3: return ShaderDataType::DMat3;
		case GL_DOUBLE_MAT3x2: return ShaderDataType::DMat3x2;
		case GL_DOUBLE_MAT3x4: return ShaderDataType::DMat3x4;

		case GL_DOUBLE_MAT4: return ShaderDataType::DMat4;
		case GL_DOUBLE_MAT4x2: return ShaderDataType::DMat4x2;
		case GL_DOUBLE_MAT4x3: return ShaderDataType::DMat4x3;
		}
		return (ShaderDataType)0;
	}

	static TBuiltInResource GetResources()
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

	EShLanguage GetEShLanguage(ShaderStage stageFlags)
	{
		switch (stageFlags)
		{
		case ShaderStage::Vertex: return EShLangVertex;
		case ShaderStage::Fragment: return EShLangFragment;
		case ShaderStage::Compute: return EShLangCompute;
		case ShaderStage::Geometry: return EShLangGeometry;
		case ShaderStage::TesselationControl: return EShLangTessControl;
		case ShaderStage::TesselationEvaluation: return EShLangTessEvaluation;
		}

		ThrowRuntime(
			"[GlslCompiler] Stage flag {0} is incompatible", String::Construct(stageFlags));
		return EShLangAnyHit;
	}
#pragma endregion

	GlslCompiler::GlslCompiler(
		const std::vector<std::string>& shaders, const std::vector<ShaderStage>& stageOrder)
	{
		if (stageOrder.size() != 0)
			RAY_MEXPECTS(shaders.size() == stageOrder.size(),
				"[GlslCompiler] Number of stages does not match number of shaders");

		static bool glslangInitialized = false;
		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}

		for (uint32_t i = 0; i < shaders.size(); ++i)
		{
			ShaderStage shaderStage;

			if (stageOrder.size() == 0)
				shaderStage = Shader::GetShaderStage(shaders[i]);
			else
				shaderStage = stageOrder[i];

			m_Shaders[shaderStage] = shaders[i];
			m_ShaderModules[shaderStage] =
				CreateShaderModule("", shaderStage, stageOrder.size() != 0);
		}
	}

	VkShaderModule GlslCompiler::CreateShaderModule(
		std::string_view preamble, ShaderStage shaderStage, bool isSrcString)
	{
		std::string moduleCode;
		std::string shaderName = "";

		if (!isSrcString)
		{
			std::string_view filepath = m_Shaders[shaderStage];
			std::optional<std::string> code = ReadFile(filepath);
			RAY_MEXPECTS(code, "[GlslCompiler] Failed to read file \"{0}\"", filepath);
			moduleCode = std::move(*code);
			shaderName = filepath;
		}
		else
			moduleCode = m_Shaders[shaderStage];

		// Convert GLSL to SPIRV
		EShLanguage language = GetEShLanguage(shaderStage);

		glslang::TProgram program;
		glslang::TShader shader(language);
		TBuiltInResource resource = GetResources();

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = EShMessages(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#ifndef NDEBUG
		messages = EShMessages(messages | EShMsgDebugInfo);
#endif

		const char* shaderNameCstr = shaderName.data();
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
			Log::Error("[GlslCompiler] Shader preproccessing failed: {0}", shader.getInfoLog());
			Log::Error("[GlslCompiler] {0}", shader.getInfoDebugLog());
		}

		if (!shader.parse(&resource, defaultVersion, true, messages, includer))
		{
			Log::Error("[GlslCompiler] Shader preproccessing failed: {0}", shader.getInfoLog());
			Log::Error("[GlslCompiler] {0}", shader.getInfoDebugLog());
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
			Log::Error("[GlslCompiler] Failed to link shader");

		program.buildReflection();

		for (uint32_t dim = 0; dim < 3; ++dim)
		{
		}

		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; --i)
		{
			LoadUniformBlock(program, (ShaderStage)shaderStage, i);
		}

		for (int32_t i = program.getNumLiveUniformVariables() - 1; i >= 0; --i)
		{
			LoadUniform(program, (ShaderStage)shaderStage, i);
		}

		for (int32_t i = 0; i < program.getNumLiveAttributes(); ++i)
		{
			LoadAttribute(program, (ShaderStage)shaderStage, i);
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
		ThrowVulkanError(vkCreateShaderModule(Graphics::Get().GetDevice(), &shaderModuleCreateInfo,
							 nullptr, &shaderModule),
			"[GlslCompiler] Failed to create shader module");

		return shaderModule;
	}

	void GlslCompiler::LoadUniform(
		const glslang::TProgram& program, ShaderStage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniform = program.getUniform(i);

		ShaderReflection::UniformData data{};
		data.binding = uniform.getBinding();
		data.dataType = GlTypeToDataType(uniform.glDefineType);
		data.size = GetShaderDataTypeSize(data.dataType);
		data.type = GlTypeToUniformType(uniform.glDefineType);
		data.offset = uniform.offset;
		data.set = uniform.getType()->getQualifier().layoutSet;

		// -1 means that it's in a block
		if (data.binding == -1)
		{
			// Insert into the already existing uniform block
			auto strings = String::Split(uniform.name, '.');
			RAY_MEXPECTS(
				strings.size() == 2, "[GlslCompiler] Invalid uniform name \"{0}\"", uniform.name);
			RAY_MEXPECTS(m_Reflections[stageFlag].HasUniformBlock(strings[0]),
				"[GlslCompiler] Uniform block \"{0}\" not found.", strings[0]);

			// Array
			if (uniform.size > 1)
			{
				Log::Warn("Parsing uniform {0} as array, be careful with alignment!", strings[1]);

				auto& uniforms = m_Reflections[stageFlag].GetUniformBlock(strings[0]).uniforms;

				for (uint32_t i = 0; i < uniform.size; ++i)
				{
					data.name = (OStringStream{} << strings[1] << '[' << i << ']').toStr();
					uniforms.emplace_back(data);

					// RAY_TODO: Align properly
					data.offset += data.size;
				}
			}
			else
			{
				data.name = strings[1];
				m_Reflections[stageFlag]
					.GetUniformBlock(strings[0])
					.uniforms.emplace_back(std::move(data));
			}
		}
		else
		{
			data.name = uniform.name;
			m_Reflections[stageFlag].AddUniform(std::move(data));
		}
	}

	void GlslCompiler::LoadUniformBlock(
		const glslang::TProgram& program, ShaderStage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& uniformBlock = program.getUniformBlock(i);

		ShaderReflection::UniformBlockData data{};
		data.binding = uniformBlock.getBinding();
		data.size = uniformBlock.size;
		data.set = uniformBlock.getType()->getQualifier().layoutSet;
		data.name = uniformBlock.name;

		if (uniformBlock.getType()->getQualifier().storage == glslang::EvqUniform)
			data.type = UniformType::UniformBuffer;
		if (uniformBlock.getType()->getQualifier().storage == glslang::EvqBuffer)
			data.type = UniformType::Storage;
		if (uniformBlock.getType()->getQualifier().layoutPushConstant)
			data.type = UniformType::Push;

		m_Reflections[stageFlag].AddUniformBlock(std::move(data));
	}

	/**
	 * If we use something like gl_VertexIndex in the shader, glslang will flag it as an attribute
	 * we need to set in the pipeline's attribute description. If the name is a reserved name like
	 * gl_VertexIndex, we won't add it to the attributes
	 * https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.60.html
	 */
	static bool IsGlslReservedName(std::string_view name)
	{
		return name == "gl_VertexIndex" || name == "gl_InstanceIndex" || name == "gl_DrawID" ||
			   name == "gl_BaseVertex" || name == "gl_BaseInstance";
	}

	void GlslCompiler::LoadAttribute(
		const glslang::TProgram& program, ShaderStage stageFlag, int32_t i)
	{
		const glslang::TObjectReflection& attribute = program.getPipeInput(i);
		const glslang::TQualifier& qualifier = attribute.getType()->getQualifier();

		if (IsGlslReservedName(attribute.name))
			return;

		ShaderReflection::AttributeData data{};
		data.location = qualifier.layoutLocation;
		// data.binding = attribute.getBinding();
		data.format = GlTypeToVkFormat(attribute.glDefineType);
		data.size = Shader::SizeOf(data.format);
		data.name = attribute.name;

		m_Reflections[stageFlag].AddAttribute(std::move(data));
	}
}  // namespace At0::Ray
