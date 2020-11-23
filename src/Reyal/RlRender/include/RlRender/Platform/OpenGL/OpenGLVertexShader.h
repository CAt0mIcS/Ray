#pragma once

#include "RlRBase.h"
#include "RlRender/VertexShader.h"

#include "OpenGLRenderBase.h"

#include <string>


namespace At0::Reyal
{
	class RLR_API OpenGLVertexShader : public VertexShader, private OpenGLRenderBase
	{
	public:
		/// <summary>
		/// OpenGL VertexShader Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the vertex shader source</param>
		OpenGLVertexShader(const std::string_view filepath, FileState state);

		/// <summary>
		/// OpenGL VertexShader Constructor
		/// </summary>
		/// <param name="name">Is the name of the vertex shader</param>
		/// <param name="vertexSrc">Is the source code from which to compile the shader</param>
		OpenGLVertexShader(const std::string_view name, const std::string_view vertexSrc);

		/// <summary>
		/// Binds the vertex shader to the pipeline
		/// </summary>
		virtual void Bind() const override;

		/// <summary>
		/// Unbinds the vertex shader from the pipeline
		/// </summary>
		virtual void Unbind() const override;

		/// <summary>
		/// Getter for the shader name
		/// </summary>
		/// <returns>The shader name</returns>
		virtual const std::string_view GetName() const override { return m_Name; }

	private:
		void Compile(const char* source);
		std::string ReadFile(const std::string_view filepath);

	private:
		std::string m_Name;
		uint32_t m_ShaderID;
	};
}


