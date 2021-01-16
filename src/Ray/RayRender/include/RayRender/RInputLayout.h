#pragma once

#include "RRBase.h"
#include <../../RayDebug/include/RayDebug/RAssert.h>

#include <initializer_list>


namespace At0::Ray
{
	class VertexShader;

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		//Mat3, Mat4,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		//Bool
	};

	inline uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
			//case ShaderDataType::Mat3:     return 4 * 3 * 3;
			//case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::UInt:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::UInt2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::UInt3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::UInt4:		return 4 * 4;
			//case ShaderDataType::Bool:     return 1;
		}

		RAY_ASSERT(false, "[ShaderDataTypeSize] Unknown Shader data type (ID={0})", (uint32_t)type);
		return 0;
	}


	struct InputElement
	{
		std::string SemanticName;
		ShaderDataType Type;
		uint32_t SemanticIndex = 0;
	};

	class RR_API InputLayout
	{
	public:
		static Ref<InputLayout> Create(std::initializer_list<InputElement> inputElements, const Ref<VertexShader>& vShader);

		virtual void Bind() = 0;

		virtual ~InputLayout() = default;
	};
}