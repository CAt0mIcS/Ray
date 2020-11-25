#pragma once

#include "RlRBase.h"

#include <string>


namespace At0::Reyal
{
	class RLR_API PixelShader
	{
	public:
		/// <summary>
		/// Creates the RendererAPI dependent PixelShader
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the shader source/binary</param>
		/// <param name="state">Specifies whether the shader file is an already compiled shader or just the shader source code</param>
		/// <returns>The created Shader</returns>
		static Ref<PixelShader> Create(const std::string_view filepath, FileState state = FileState::Compiled);

		/// <summary>
		/// Creates the RendererAPI dependent PixelShader, also compiles the pixel source
		/// </summary>
		/// <param name="name">Is the shader name</param>
		/// <param name="pixelSrc">Is the source code of the pixel shader</param>
		/// <returns>The created Shader</returns>
		static Ref<PixelShader> Create(const std::string_view name, const std::string_view pixelSrc);

		/// <summary>
		/// Binds the pixel shader to the pipeline
		/// </summary>
		virtual void Bind() const = 0;

		/// <summary>
		/// Unbinds the pixel shader from the pipeling;
		/// </summary>
		virtual void Unbind() const = 0;

		/// <summary>
		/// Getter for the shader name
		/// </summary>
		/// <returns>The shader name</returns>
		virtual const std::string_view GetName() const = 0;

	protected:
		/// <summary>
		/// Default PixelShader Constructor
		/// </summary>
		PixelShader() = default;
	};
}

