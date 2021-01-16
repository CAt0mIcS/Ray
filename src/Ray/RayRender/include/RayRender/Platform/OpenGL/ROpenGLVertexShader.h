#pragma once

#include "../../RVertexShader.h"


namespace At0::Ray
{
	class RR_API OpenGLVertexShader : public VertexShader
	{
		friend class OpenGLInputLayout;
	public:
		OpenGLVertexShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;

	private:
		// Shader buffer access functions for the InputLayout
		void* GetBufferPointer() const;
		size_t GetBufferSize() const;
	};
}
