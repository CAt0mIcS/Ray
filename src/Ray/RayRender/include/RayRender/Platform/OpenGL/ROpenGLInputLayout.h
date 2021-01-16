#pragma once

#include "../../RInputLayout.h"


namespace At0::Ray
{
	class VertexShader;

	uint32_t ShaderDataTypeSize(ShaderDataType type);

	class RR_API OpenGLInputLayout : public InputLayout
	{
	public:
		OpenGLInputLayout(std::initializer_list<InputElement> inputElements, const VertexShader* vShader);

		virtual void Bind() override;
	};
}
