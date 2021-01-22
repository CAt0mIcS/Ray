#pragma once

#include "../../RRBase.h"

#include <string_view>


namespace At0::Ray
{
	class RR_API OpenGLShaderBase
	{
	public:
		OpenGLShaderBase() = default;
		virtual ~OpenGLShaderBase() = default;

		uint32_t GetProgram() const { return m_ShaderProgram; }

	protected:
		void BindProgram();
		void OnVertexShaderCompilationFailed(int success);
		void OnPixelShaderCompilationFailed(int success);
		void CreateProgram();
		std::string ReadShaderSource(std::string_view filepath);

	private:
		void OnShaderLinkageFailed(int success);

	protected:
		uint32_t m_Shader = (uint32_t)-1;

	private:
		uint32_t m_ShaderProgram;
	};
}