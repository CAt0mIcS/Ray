#pragma once

#include "../../RBase.h"

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
	class VertexLayout;

	inline std::optional<std::string> ReadFile(const std::filesystem::path& filepath)
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


	class RAY_EXPORT Shader
	{
	public:
		enum class Stage
		{
			Vertex,
			TesselationControl,
			TesselationEvaluation,
			Geometry,
			Fragment,
			Compute,
		};

		class Attributes
		{
			friend class Shader;

		public:
			struct AttributeData
			{
				// The location specified in the shader, e.g. "layout(location = 0) in vec3 inPos"
				uint32_t location;

				// No idea what that is tbh, it's usually 0 or (uint32_t)-1
				uint32_t binding;

				// The size in bytes of the type to input, e.g. vec3 == 3 * sizeof(float)
				uint32_t size;

				// The format of the data to input
				VkFormat format;
			};

		public:
			void Emplace(std::string_view attributeName, const AttributeData& data);

		private:
			std::unordered_map<std::string, AttributeData> m_Attributes;
		};

		class Uniforms
		{
			friend class Shader;

		public:
			struct UniformData
			{
				// The binding specified in the shader, e.g. "layout(binding = 0) uniform..."
				uint32_t binding;

				// The global offset in the global uniform buffer
				uint32_t offset;

				// The size in bytes of this uniform
				uint32_t size;
			};

		public:
			void Emplace(std::string_view uniformName, const UniformData& data);
			std::optional<Uniforms::UniformData> Get(std::string_view uniformName) const;

		private:
			std::unordered_map<std::string, UniformData> m_Uniforms;
		};

		class UniformBlocks
		{
			friend class Shader;

		public:
			struct UniformBlockData
			{
				// The binding specified in the shader, e.g. "layout(binding = 0) uniform..."
				uint32_t binding;

				// The size in bytes of the entire uniform block
				uint32_t size;

				// All the uniforms in the uniform block
				Uniforms uniforms;
			};

		public:
			void Emplace(std::string_view uniformBlockName, const UniformBlockData& data);
			std::optional<UniformBlocks::UniformBlockData> Get(
				std::string_view uniformBlockName) const;


		private:
			std::unordered_map<std::string, UniformBlockData> m_UniformBlocks;
		};

	public:
		Shader();
		~Shader();

		static VkFormat GlTypeToVkFormat(int32_t type);
		VkShaderModule CreateShaderModule(const std::filesystem::path& moduleName,
			std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag);
		static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& filepath);

		void CreateReflection();

		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions() const;
		std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions(
			uint32_t binding = 0) const;
		const VertexLayout& GetVertexLayout() const { return *m_VertexLayout; }
		static Shader::Stage ToShaderStage(VkShaderStageFlags stageFlags);

		std::optional<Shader::UniformBlocks> GetUniformBlocks(Shader::Stage stage) const;
		std::optional<Shader::Uniforms> GetUniforms(Shader::Stage stage) const;
		std::vector<Shader::Stage> GetLiveShaderStages() const;

		const auto& GetDescriptorSetLayoutBindings() const { return m_DescriptorSetLayoutBindings; }
		const auto& GetDescriptorPoolSizes() const { return m_DescriptorPoolSizes; }

	private:
		void LoadUniform(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadUniformBlock(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadAttribute(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);

	private:
		static int32_t ComputeSize(const glslang::TType* ttype);
		static uint32_t CalculateNextOffset(uint32_t uniformSize);

	private:
		struct ShaderData
		{
			Attributes attributes;
			Uniforms uniforms;
			UniformBlocks uniformBlocks;
		};

		std::unordered_map<Shader::Stage, ShaderData> m_ShaderData;
		Scope<VertexLayout> m_VertexLayout;

		std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
		std::vector<VkDescriptorPoolSize> m_DescriptorPoolSizes;
	};
}  // namespace At0::Ray
