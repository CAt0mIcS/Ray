
#pragma once

#include "../../../RBase.h"
#include "../../../Core/RResourceManager.h"
#include "RShaderTypes.h"
#include "RShaderReflection.h"

#include <vulkan/vulkan_core.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>


namespace glslang
{
	class TProgram;
	class TType;
}  // namespace glslang


namespace At0::Ray
{
	class RAY_EXPORT Shader : public Resource
	{
	public:
		enum FileType : uint16_t
		{
			GLSLFile = 1,
			CompiledFile,
			GLSLString,
			// CompiledString
		};

	public:
		static Ref<Shader> FromSourceFile(
			std::vector<std::string> shaders, const std::vector<std::string>& reflections = {});

		static Ref<Shader> FromCompiledFile(
			std::vector<std::string> shaders, const std::vector<std::string>& reflections);

		static Ref<Shader> FromSourceString(std::vector<std::string> shaders,
			const std::vector<ShaderStage>& stageOrder,
			const std::vector<std::string>& reflections = {});

		Shader(std::vector<std::string> shaders, FileType flags,
			const std::vector<std::string>& reflections = {},
			const std::vector<ShaderStage>& stageOrder = {});

		~Shader();

		static ShaderStage GetShaderStage(const std::filesystem::path& filepath);
		static uint32_t SizeOf(VkFormat format);

		std::vector<ShaderStage> GetLiveShaderStages() const;

		const auto& GetDescriptorSetLayoutBindings() const { return m_DescriptorSetLayoutBindings; }
		const auto& GetDescriptorPoolSizes() const { return m_DescriptorPoolSizes; }
		std::vector<VkPushConstantRange> GetPushConstantRanges() const;
		const auto& GetReflection(ShaderStage stage) const { return m_Reflections.at(stage); }
		const auto& GetReflections() const { return m_Reflections; }
		const auto& GetShaderModules() const { return m_ShaderModules; }
		const auto& GetFilepaths() const { return m_Filepaths; }

		std::vector<VkVertexInputBindingDescription> GetVertexInputBindings(
			uint32_t binding = 0) const;
		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributes(
			uint32_t binding = 0) const;

	private:
		void CreateReflection();

	private:
		static void IncrementDescriptorPool(
			std::unordered_map<VkDescriptorType, uint32_t>& descriptorPoolCounts,
			VkDescriptorType type);

	private:
		std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>
			m_DescriptorSetLayoutBindings;
		std::vector<VkDescriptorPoolSize> m_DescriptorPoolSizes;

		std::unordered_map<ShaderStage, ShaderReflection> m_Reflections;
		std::unordered_map<ShaderStage, VkShaderModule> m_ShaderModules;

		std::vector<std::string> m_Filepaths;
	};
}  // namespace At0::Ray
