#include "Rpch.h"
#include "RShader.h"

#include "Core/RDynamicVertex.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"
#include "Utils/RLogger.h"
#include "Utils/RString.h"

#include "RGlslCompiler.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Buffers/RUniformBuffer.h"


namespace At0::Ray
{
	Ref<Shader> Shader::Acquire(
		std::vector<std::string> shaders, const std::vector<std::string>& reflections, Flags flags)
	{
		return Resources::Get().EmplaceIfNonExistent<Shader>(
			std::accumulate(shaders.begin(), shaders.end(), std::string{}), std::move(shaders),
			reflections, flags);
	}

	Shader::Shader(
		std::vector<std::string> shaders, const std::vector<std::string>& reflections, Flags flags)
		: m_Filepaths(std::move(shaders))
	{
		if (flags == Shader::GLSL)
		{
			GlslCompiler compiler(m_Filepaths);
			m_Reflections = std::move(compiler.AcquireReflections());
			m_ShaderModules = std::move(compiler.AcquireShaderModules());
		}
		else if (flags == Shader::Compiled)
		{
			RAY_MEXPECTS(m_Filepaths.size() == reflections.size(),
				"[Shader] Number of shaders ({0}) doesn't match number of reflections ({1})",
				m_Filepaths.size(), reflections.size());
			for (uint32_t i = 0; i < m_Filepaths.size(); ++i)
			{
				ShaderStage stage = GetShaderStage(m_Filepaths[i]);
				m_Reflections[stage] = ShaderReflection{ reflections[i] };

				std::ifstream reader(m_Filepaths[i], std::ios::ate | std::ios::binary);
				std::vector<char> code(reader.tellg());
				reader.seekg(std::ios::beg);
				reader.read(code.data(), code.size());
				reader.close();

				VkShaderModule shaderModule;
				VkShaderModuleCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = (uint32_t)code.size();
				createInfo.pCode = (uint32_t*)code.data();

				ThrowVulkanError(vkCreateShaderModule(Graphics::Get().GetDevice(), &createInfo,
									 nullptr, &shaderModule),
					"[Shader] Failed to create shader module from file \"{0}\"", m_Filepaths[i]);
				m_ShaderModules[stage] = shaderModule;
			}
		}
		else
			RAY_ASSERT(false, "[Shader] Invalid flag configuration ({0})", (uint32_t)flags);

		CreateReflection();
	}

	Ref<Shader> Shader::FromCompiled(
		std::vector<std::string> shaders, std::vector<std::string> reflections)
	{
		// Search for reflection files
		if (reflections.empty())
		{
			for (const std::string& shader : shaders)
			{
				if (shader.ends_with(".spv"))
					reflections.emplace_back(
						std::filesystem::path{ shader }.replace_extension("rreflection").string());
				else
					reflections.emplace_back(shader + ".rreflection");

				if (!std::filesystem::exists(reflections.back()))
					ThrowRuntime("[Shader] Reflection file \"{0}\" not found.", reflections.back());
			}
		}

		return MakeRef<Shader>(std::move(shaders), reflections, Shader::Compiled);
	}

	Ref<Shader> Shader::FromGlsl(std::vector<std::string> shaders)
	{
		return MakeRef<Shader>(std::move(shaders), std::vector<std::string>{}, Shader::GLSL);
	}

	Shader::~Shader()
	{
		uint16_t i = 0;
		for (const auto& [stage, reflection] : m_Reflections)
		{
			std::string path;
			if (m_Filepaths[i].ends_with(".spv"))
				path = std::filesystem::path{ m_Filepaths[i] }
						   .replace_extension("rreflection")
						   .string();
			else
				path = m_Filepaths[i] + ".rreflection";

			reflection.WriteToFile(path);
			++i;
		}

		for (const auto [stage, shaderModule] : m_ShaderModules)
			vkDestroyShaderModule(Graphics::Get().GetDevice(), shaderModule, nullptr);
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

	ShaderStage Shader::GetShaderStage(const std::filesystem::path& filepath)
	{
		std::string fileExt;
		if (filepath.extension() == ".spv")
		{
			std::filesystem::path shaderPath = filepath;
			shaderPath.replace_extension();
			fileExt = shaderPath.extension().string();
		}
		else
			fileExt = filepath.extension().string();

		std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);

		if (fileExt == ".comp")
			return ShaderStage::Compute;
		if (fileExt == ".vert")
			return ShaderStage::Vertex;
		if (fileExt == ".tesc")
			return ShaderStage::TesselationControl;
		if (fileExt == ".tese")
			return ShaderStage::TesselationEvaluation;
		if (fileExt == ".geom")
			return ShaderStage::Geometry;
		if (fileExt == ".frag")
			return ShaderStage::Fragment;
		return ShaderStage::All;
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

		for (auto& [shaderStage, reflection] : m_Reflections)
		{
			// Sort attributes by location and add their format to the vertex layout
			std::sort(reflection.GetAttributes().begin(), reflection.GetAttributes().end(),
				[](const ShaderReflection::AttributeData& l,
					const ShaderReflection::AttributeData& r) { return l.location < r.location; });

			for (auto& uniformBlock : reflection.GetUniformBlocks())
			{
				VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

				switch (uniformBlock.type)
				{
				case UniformType::UniformBuffer:
				case UniformType::CombinedImageSampler:
				{
					descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

					m_DescriptorSetLayoutBindings[uniformBlock.set].emplace_back(
						UniformBuffer::GetDescriptorSetLayout((uint32_t)uniformBlock.binding,
							descriptorType, (VkShaderStageFlags)shaderStage, 1));
					break;
				}
				}

				IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
			}

			for (const auto& uniform : reflection.GetUniforms())
			{
				VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
				switch (uniform./*glType*/ type)
				{
				case UniformType::CombinedImageSampler:
				{
					descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					VkDescriptorSetLayoutBinding samplerLayoutBinding{};
					samplerLayoutBinding.binding = uniform.binding;
					samplerLayoutBinding.descriptorCount = 1;
					samplerLayoutBinding.descriptorType = descriptorType;
					samplerLayoutBinding.pImmutableSamplers = nullptr;
					samplerLayoutBinding.stageFlags = (VkShaderStageFlags)shaderStage;

					descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					m_DescriptorSetLayoutBindings[uniform.set].emplace_back(samplerLayoutBinding);
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

	std::vector<ShaderStage> Shader::GetLiveShaderStages() const
	{
		std::vector<ShaderStage> stages;
		stages.reserve(m_Reflections.size());

		for (auto& it : m_Reflections)
		{
			stages.emplace_back(it.first);
		}

		return stages;
	}

	std::vector<VkPushConstantRange> Shader::GetPushConstantRanges() const
	{
		std::vector<VkPushConstantRange> pushConstantRanges;

		uint32_t currentOffset = 0;
		for (const auto& [shaderStage, reflection] : m_Reflections)
		{
			for (const auto& uniformBlock : reflection.GetUniformBlocks())
			{
				if (uniformBlock.type != UniformType::Push)
					continue;

				VkPushConstantRange pushConstantRange{};
				pushConstantRange.stageFlags = (VkShaderStageFlags)shaderStage;
				pushConstantRange.offset = currentOffset;
				pushConstantRange.size = uniformBlock.size;
				pushConstantRanges.emplace_back(pushConstantRange);

				currentOffset += uniformBlock.size;
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
}  // namespace At0::Ray
