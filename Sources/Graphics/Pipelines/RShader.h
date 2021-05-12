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

		enum class DataType
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

				/**
				 * The name of the attribute (layout(location = 0) in vec3 inPos) --> inPos
				 */
				std::string attributeName;
			};

		public:
			void Emplace(const AttributeData& data) { m_Attributes.emplace_back(data); }

			AttributeData& operator[](uint32_t i) { return m_Attributes[i]; }
			const AttributeData& operator[](uint32_t i) const { return m_Attributes[i]; }

			const auto begin() const { return m_Attributes.begin(); }
			const auto end() const { return m_Attributes.end(); }

			auto begin() { return m_Attributes.begin(); }
			auto end() { return m_Attributes.end(); }

		private:
			std::vector<AttributeData> m_Attributes;
		};

		class Uniforms
		{
			friend class Shader;

		public:
			struct UniformData
			{
				/*
				 * The binding specified in the shader, e.g. "layout(binding = 0) uniform.."
				 * If the uniform is in a block this value is undefined
				 */
				uint32_t binding;

				/*
				 * The offset in the uniform block or 0 if the uniform is not in a uniform block
				 */
				uint32_t offset;

				/*
				 * The size in bytes of this uniform
				 */
				uint32_t size;

				/*
				 * The type ID of the gl type
				 */
				int32_t glType;

				/**
				 * The set specified in the shader layout (layout(set = 0, binding = x)).
				 * If the uniform is in a uniform block, this value is undefined
				 */
				uint32_t set;

				std::string uniformName;
			};

		public:
			void Emplace(const UniformData& data) { m_Uniforms.emplace_back(data); }
			const Uniforms::UniformData* Get(std::string_view uniformName) const;
			bool HasUniform(std::string_view name) const;

			const auto begin() const { return m_Uniforms.begin(); }
			const auto end() const { return m_Uniforms.end(); }

			auto begin() { return m_Uniforms.begin(); }
			auto end() { return m_Uniforms.end(); }

		private:
			std::vector<UniformData> m_Uniforms;
		};

		class UniformBlocks
		{
			friend class Shader;

		public:
			enum class Type
			{
				None,
				Uniform,
				Storage,
				Push
			};

			struct UniformBlockData
			{
				// The binding specified in the shader, e.g. "layout(binding = 0) uniform.."
				uint32_t binding;

				// The size in bytes of the entire uniform block
				uint32_t size;

				// All the uniforms in the uniform block
				Uniforms uniforms;

				// The type of the uniform block
				Type type;

				/**
				 * The set specified in the shader layout (layout(set = 0, binding = x))
				 */
				uint32_t set;

				/**
				 * The name of the uniform block
				 */
				std::string uniformBlockName;
			};

		public:
			void Emplace(const UniformBlockData& data) { m_UniformBlocks.emplace_back(data); }
			const UniformBlocks::UniformBlockData* Get(std::string_view uniformBlockName) const;
			bool HasUniformBlock(std::string_view name) const;
			bool HasUniform(std::string_view name) const;

			const auto begin() const { return m_UniformBlocks.begin(); }
			const auto end() const { return m_UniformBlocks.end(); }

			auto begin() { return m_UniformBlocks.begin(); }
			auto end() { return m_UniformBlocks.end(); }

		private:
			std::vector<UniformBlockData> m_UniformBlocks;
		};

	public:
		Shader();
		~Shader();

		static VkFormat GlTypeToVkFormat(int32_t type);
		VkShaderModule CreateShaderModule(const std::filesystem::path& moduleName,
			std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag);
		static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& filepath);
		static uint32_t SizeOf(VkFormat format);

		void CreateReflection();

		bool HasUniform(std::string_view name, Shader::Stage stage) const;

		static Shader::Stage ToShaderStage(VkShaderStageFlags stageFlags);
		static VkShaderStageFlags ToVkShaderStage(Shader::Stage stageFlags);

		const Shader::UniformBlocks* GetUniformBlocks(Shader::Stage stage) const;
		const Shader::Uniforms* GetUniforms(Shader::Stage stage) const;
		const Shader::Attributes* GetAttributes(Shader::Stage stage) const;
		std::vector<Shader::Stage> GetLiveShaderStages() const;

		const auto& GetDescriptorSetLayoutBindings() const { return m_DescriptorSetLayoutBindings; }
		const auto& GetDescriptorPoolSizes() const { return m_DescriptorPoolSizes; }
		std::vector<VkPushConstantRange> GetPushConstantRanges() const;

	private:
		void LoadUniform(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadUniformBlock(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);
		void LoadAttribute(const glslang::TProgram& program, Shader::Stage stageFlag, int32_t i);

	private:
		static int32_t ComputeSize(const glslang::TType* ttype);
		static void IncrementDescriptorPool(
			std::unordered_map<VkDescriptorType, uint32_t>& descriptorPoolCounts,
			VkDescriptorType type);

	private:
		struct ShaderData
		{
			Attributes attributes;
			Uniforms uniforms;
			UniformBlocks uniformBlocks;
		};

		std::unordered_map<Shader::Stage, ShaderData> m_ShaderData;

		std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>
			m_DescriptorSetLayoutBindings;
		std::vector<VkDescriptorPoolSize> m_DescriptorPoolSizes;
	};
}  // namespace At0::Ray
