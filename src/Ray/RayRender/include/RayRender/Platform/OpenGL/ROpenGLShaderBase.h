#pragma once

#include "../../RRBase.h"


namespace At0::Ray
{
	class RR_API OpenGLShaderBase
	{
	public:
		OpenGLShaderBase() = default;
		virtual ~OpenGLShaderBase() = default;

	protected:
		void AttachVertexShader(uint32_t shaderID);
		void AttachPixelShader(uint32_t shaderID);

		void OnShaderCompilationFailed(int success);

	private:
		void OnShaderLinkageFailed(int success);
		void CreateProgram();

	private:
		uint32_t m_VertexShader = (uint32_t)-1;
		uint32_t m_PixelShader = (uint32_t)-1;

		uint32_t m_ShaderProgram;
	};
}