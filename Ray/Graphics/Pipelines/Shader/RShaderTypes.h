#pragma once

#include "../../../Core/RMath.h"

namespace At0::Ray
{
	enum class ShaderDataType
	{
		Float = 1,
		Int,
		UInt,

		Vec2,
		Vec3,
		Vec4,

		Mat3,
		Mat4,
		Bool
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

	inline uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::UInt:
		case ShaderDataType::Bool: return 4;
		case ShaderDataType::Vec2: return sizeof(Float2);
		case ShaderDataType::Vec3: return sizeof(Float3);
		case ShaderDataType::Vec4: return sizeof(Float4);
		case ShaderDataType::Mat3: return sizeof(Matrix3);
		case ShaderDataType::Mat4: return sizeof(Matrix);
		}
		return 0;
	}
}  // namespace At0::Ray
