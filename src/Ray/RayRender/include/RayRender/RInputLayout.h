#pragma once

#include "RRBase.h"

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