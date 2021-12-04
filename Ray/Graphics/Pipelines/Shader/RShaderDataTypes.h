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
		Mat4
	};

	inline uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::UInt: return 4;
		case ShaderDataType::Vec2: return sizeof Float2;
		case ShaderDataType::Vec3: return sizeof Float3;
		case ShaderDataType::Vec4: return sizeof Float4;
		case ShaderDataType::Mat3: return sizeof Matrix3;
		case ShaderDataType::Mat4: return sizeof Matrix;
		}
		return 0;
	}
}  // namespace At0::Ray
