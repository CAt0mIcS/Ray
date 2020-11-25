#pragma once

#include "RlRBase.h"

#include <string>


namespace At0::Reyal
{
	class RLR_API VertexShader
	{
	public:
		/// <summary>
		/// Creates the RendererAPI dependent VertexShader
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the shader source/binary</param>
		/// <param name="state">Specifies whether the shader file is an already compiled shader or just the shader source code</param>
		/// <returns>The created Shader</returns>
		static Ref<VertexShader> Create(const std::string_view filepath, FileState state = FileState::Compiled);

		/// <summary>
		/// Creates the RendererAPI dependent VertexShader, also compiles the vertex source
		/// </summary>
		/// <param name="name">Is the shader name</param>
		/// <param name="vertexSrc">Is the source code of the vertex shader</param>
		/// <returns>The created Shader</returns>
		static Ref<VertexShader> Create(const std::string_view name, const std::string_view vertexSrc);

		/// <summary>
		/// Binds the vertex shader to the pipeline
		/// </summary>
		virtual void Bind() const = 0;

		/// <summary>
		/// Unbinds the vertex shader from the pipeling;
		/// </summary>
		virtual void Unbind() const = 0;

		/// <summary>
		/// Getter for the shader name
		/// </summary>
		/// <returns>The shader name</returns>
		virtual const std::string_view GetName() const = 0;

		/// <summary>
		/// Getter for the compiled shader code
		/// </summary>
		/// <returns>The buffer to the compiled shader resource</returns>
		virtual void* GetBufferPointer() const;

		/// <summary>
		/// Getter for the compiled shader buffer size
		/// </summary>
		/// <returns>The size of the buffer</returns>
		virtual size_t GetBufferSize() const;

	protected:
		/// <summary>
		/// Default VertexShader Constructor
		/// </summary>
		VertexShader() = default;
	};
}

