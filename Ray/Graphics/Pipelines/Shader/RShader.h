#pragma once

#include "Ray/RBase.h"
#include "RShaderReflection.h"

#include <vulkan/vulkan_core.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <optional>
#include <fstream>


namespace glslang
{
	class TProgram;
	class TType;
}  // namespace glslang


namespace At0::Ray
{
	enum class ShaderStage
	{
		Vertex = VK_SHADER_STAGE_VERTEX_BIT,
		TesselationControl = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
		TesselationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
		Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
		Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
		Compute = VK_SHADER_STAGE_COMPUTE_BIT,
		All = VK_SHADER_STAGE_ALL
	};

	enum class ShaderDataType
	{
		Float,
		Int,
		UInt,

		Vec2,
		Vec3,
		Vec4,

		Mat3,
		Mat4
	};

	class RAY_EXPORT Shader
	{
	public:
		enum Flags : uint16_t
		{
			GLSL = 1,
			Compiled = 2
		};

	public:
		Shader(std::vector<std::string> shaders, const std::vector<std::string>& reflections = {},
			Flags flags = GLSL);

		static Ref<Shader> FromCompiled(
			std::vector<std::string> shaders, std::vector<std::string> reflections = {});
		static Ref<Shader> FromGlsl(std::vector<std::string> shaders);

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

		static std::string GetUID(const std::vector<std::string>& shaders);

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
