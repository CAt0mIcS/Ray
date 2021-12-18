#pragma once

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	enum class ShaderDataType
	{
		Float = 1,
		Vec2,
		Vec3,
		Vec4,

		Int,
		IVec2,
		IVec3,
		IVec4,

		UInt,
		UVec2,
		UVec3,
		UVec4,

		Bool,
		BVec2,
		BVec3,
		BVec4,

		Double,
		DVec2,
		DVec3,
		DVec4,

		Mat2,
		Mat2x3,
		Mat2x4,

		Mat3,
		Mat3x2,
		Mat3x4,

		Mat4,
		Mat4x2,
		Mat4x3,

		DMat2,
		DMat2x3,
		DMat2x4,

		DMat3,
		DMat3x2,
		DMat3x4,

		DMat4,
		DMat4x2,
		DMat4x3
	};

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

	enum class AttributeType
	{
		Position,
		UV,
		Normal,
		Tangent
	};

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

	/**
	 * Predefined uniform block tags in the shader
	 */
	struct UniformBlockTag
	{
		static constexpr const char* PerSceneData = "PerSceneData";
		static constexpr const char* PerObjectData = "PerObjectData";
		static constexpr const char* Shading = "Shading";
	};

	/**
	 * Predefined uniform tags in the shader
	 */
	struct UniformTag
	{
		static constexpr const char* DiffuseMapSampler = "samplerDiffuse";
		static constexpr const char* SpecularMapSampler = "samplerSpecular";
		static constexpr const char* NormalMapSampler = "samplerNormal";
		static constexpr const char* HeightMapSampler = "samplerHeight";

		static constexpr const char* Color = "color";
	};

	constexpr inline uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::UInt:
		case ShaderDataType::Bool: return 4;
		case ShaderDataType::Vec2:
		case ShaderDataType::IVec2:
		case ShaderDataType::UVec2:
		case ShaderDataType::BVec2:
		case ShaderDataType::Double: return 8;
		case ShaderDataType::Vec3:
		case ShaderDataType::IVec3:
		case ShaderDataType::UVec3:
		case ShaderDataType::BVec3: return 12;
		case ShaderDataType::Vec4:
		case ShaderDataType::IVec4:
		case ShaderDataType::UVec4:
		case ShaderDataType::BVec4:
		case ShaderDataType::DVec2: return 16;
		case ShaderDataType::DVec3: return 24;
		case ShaderDataType::DVec4: return 32;
		case ShaderDataType::Mat2: return 2 * 2 * 4;
		case ShaderDataType::Mat2x3: return 2 * 3 * 4;
		case ShaderDataType::Mat2x4: return 2 * 4 * 4;
		case ShaderDataType::Mat3: return 3 * 3 * 4;
		case ShaderDataType::Mat3x2: return 3 * 2 * 4;
		case ShaderDataType::Mat3x4: return 3 * 4 * 4;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Mat4x2: return 4 * 2 * 4;
		case ShaderDataType::Mat4x3: return 4 * 3 * 4;
		case ShaderDataType::DMat2: return 2 * 2 * 8;
		case ShaderDataType::DMat2x3: return 2 * 3 * 8;
		case ShaderDataType::DMat2x4: return 2 * 4 * 8;
		case ShaderDataType::DMat3: return 3 * 3 * 8;
		case ShaderDataType::DMat3x2: return 3 * 2 * 8;
		case ShaderDataType::DMat3x4: return 3 * 4 * 8;
		case ShaderDataType::DMat4: return 4 * 4 * 8;
		case ShaderDataType::DMat4x2: return 4 * 2 * 8;
		case ShaderDataType::DMat4x3: return 4 * 3 * 8;
		}
		return 0;
	}
}  // namespace At0::Ray
