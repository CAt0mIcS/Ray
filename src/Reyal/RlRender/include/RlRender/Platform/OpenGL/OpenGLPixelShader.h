#pragma once

#include "RlRBase.h"
#include "RlRender/PixelShader.h"

#include "OpenGLRenderBase.h"

#include <string>


namespace At0::Reyal
{
	class RLR_API OpenGLPixelShader : public PixelShader, private OpenGLRenderBase
	{
	public:
		/// <summary>
		/// OpenGL PixelShader Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the pixel shader source</param>
		OpenGLPixelShader(const std::string_view filepath, FileState state);

		/// <summary>
		/// OpenGL PixelShader Constructor
		/// </summary>
		/// <param name="name">Is the name of the pixel shader</param>
		/// <param name="pixelSrc">Is the source code from which to compile the shader</param>
		OpenGLPixelShader(const std::string_view name, const std::string_view pixelSrc);

		/// <summary>
		/// Binds the pixel shader to the pipeline
		/// </summary>
		virtual void Bind() const override;

		/// <summary>
		/// Unbinds the pixel shader from the pipeline
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
	};
}


