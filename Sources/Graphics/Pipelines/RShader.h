#pragma once

#include "../../RBase.h"
#include "../../Core/RVertex.h"

#include <vulkan/vulkan_core.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <optional>


namespace glslang
{
	class TProgram;
	class TType;
}  // namespace glslang


namespace At0::Ray
{
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
				uint32_t location;
				uint32_t binding;
				uint32_t size;
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
			};

		public:
			void Emplace(std::string_view uniformName, const UniformData& data);

		private:
			std::unordered_map<std::string, UniformData> m_Uniforms;
		};

		class UniformBlocks
		{
			friend class Shader;

		public:
			struct UniformBlockData
			{
				Uniforms uniforms;
			};

		public:
			void Emplace(std::string_view uniformBlockName, const UniformBlockData& data);

		private:
			std::unordered_map<std::string, UniformBlockData> m_UniformBlocks;
		};

	public:
		Shader();
		~Shader() = default;

		static VkFormat GlTypeToVkFormat(int32_t type);
		VkShaderModule CreateShaderModule(const std::filesystem::path& moduleName,
			std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag);
		static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& filepath);
		static EShLanguage GetEShLanguage(VkShaderStageFlags stageFlags);

		void CreateReflection();

		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributeDescriptions() const;
		std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions(
			uint32_t binding = 0) const;
		const VertexLayout& GetVertexLayout() const { return m_VertexLayout; }
		static Shader::Stage ToShaderStage(VkShaderStageFlags stageFlags);

	private:
		void LoadUniform(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadUniformBlock(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadAttribute(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);

	private:
		static int32_t ComputeSize(const glslang::TType* ttype);

	private:
		struct ShaderData
		{
			Attributes attributes;
			Uniforms uniforms;
			UniformBlocks uniformBlocks;
		};

		std::unordered_map<Shader::Stage, ShaderData> m_ShaderData;
		VertexLayout m_VertexLayout;
	};
}  // namespace At0::Ray
