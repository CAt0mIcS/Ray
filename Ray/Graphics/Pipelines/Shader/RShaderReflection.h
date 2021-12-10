#pragma once

#include "../../../RBase.h"

#include <unordered_map>

#include <../../Extern/rapidjson/include/rapidjson/document.h>


namespace At0::Ray
{
	/**
	 * Specifies the different types of shader uniform inputs
	 */
	enum class UniformType
	{
		None,
		UniformBuffer,
		CombinedImageSampler,
		Storage,
		Push
	};


	/**
	 * Reflection data for one shader file
	 */
	class RAY_EXPORT ShaderReflection
	{
	public:
		struct AttributeData
		{
			std::string name;

			// The location specified in the shader, e.g. "layout(location = 0) in vec3 inPos"
			uint32_t location;

			// The size in bytes of the type to input, e.g. vec3 == 3 * sizeof(float)
			uint32_t size;

			// The format of the data to input
			VkFormat format;
		};

		struct UniformData
		{
			std::string name;

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

			// The type of the uniform
			UniformType type;

			/**
			 * The set specified in the shader layout (layout(set = 0, binding = x)).
			 * If the uniform is in a uniform block, this value is undefined
			 */
			uint32_t set;
		};

		struct UniformBlockData
		{
			std::string name;

			// The binding specified in the shader, e.g. "layout(binding = 0) uniform.."
			uint32_t binding;

			// The size in bytes of the entire uniform block
			uint32_t size;

			// All the uniforms in the uniform block
			std::vector<UniformData> uniforms;

			// The type of the uniform block
			UniformType type;

			/**
			 * The set specified in the shader layout (layout(set = 0, binding = x))
			 */
			uint32_t set;

			const UniformData& GetUniform(std::string_view name) const;
			UniformData& GetUniform(std::string_view name);
		};

	public:
		/**
		 * Generate reflection from reflection file
		 */
		ShaderReflection(const std::string& filepath);
		ShaderReflection() = default;

		void AddAttribute(AttributeData data) { m_Attributes.emplace_back(std::move(data)); }
		void AddUniform(UniformData data) { m_Uniforms.emplace_back(std::move(data)); }
		void AddUniformBlock(UniformBlockData data);

		bool HasAttribute(std::string_view name) const;
		bool HasUniform(std::string_view name, bool includeUniformBlocks = false) const;
		bool HasUniformBlock(std::string_view name) const;

		/**
		 * Uses path to uniform to check if it exists, e.g. "Shader.color" -->
		 * UniformBlock: "Shader"
		 * Uniform in "Shader": "color"
		 */
		bool HasPathedUniform(std::string_view name) const;

		auto& GetAttributes() const { return m_Attributes; }
		auto& GetAttributes() { return m_Attributes; }
		auto& GetUniforms() const { return m_Uniforms; }
		auto& GetUniforms() { return m_Uniforms; }
		auto& GetUniformBlocks() const { return m_UniformBlocks; }
		auto& GetUniformBlocks() { return m_UniformBlocks; }

		const AttributeData& GetAttribute(std::string_view name) const;
		const UniformData& GetUniform(std::string_view name) const;
		const UniformBlockData& GetUniformBlock(std::string_view name) const;
		AttributeData& GetAttribute(std::string_view name);
		UniformData& GetUniform(std::string_view name);
		UniformBlockData& GetUniformBlock(std::string_view name);

		const AttributeData* TryGetAttribute(std::string_view name) const;
		const UniformData* TryGetUniform(std::string_view name) const;
		const UniformBlockData* TryGetUniformBlock(std::string_view name) const;
		AttributeData* TryGetAttribute(std::string_view name);
		UniformData* TryGetUniform(std::string_view name);
		UniformBlockData* TryGetUniformBlock(std::string_view name);

		void WriteToFile(const std::string& filepath) const;

	private:
		AttributeData LoadAttribute(const rapidjson::Value& data);
		UniformBlockData LoadUniformBlock(const rapidjson::Value& data);
		UniformData LoadUniform(const rapidjson::Value& data);

	private:
		std::vector<AttributeData> m_Attributes;
		std::vector<UniformData> m_Uniforms;
		std::vector<UniformBlockData> m_UniformBlocks;
	};
}  // namespace At0::Ray
