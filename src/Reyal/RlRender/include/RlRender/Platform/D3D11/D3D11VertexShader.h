#ifndef RLR_D3D11VERTEXSHADER
#define RLR_D3D11VERTEXSHADER

#ifdef _WIN32


#include "RlRBase.h"
#include "D3D11RenderBase.h"

#include "RlRender/VertexShader.h"

#include <wrl.h>


namespace At0::Reyal
{
	class RLR_API D3D11VertexShader : public VertexShader, private D3D11RenderBase
	{
	public:
		/// <summary>
		/// D3D11 VertexShader Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the ALREADY COMPILED vertex shader</param>
		D3D11VertexShader(const std::string_view filepath, FileState state);

		/// <summary>
		/// D3D11 VertexShader Constructor
		/// </summary>
		/// <param name="name">Is the name of the vertex shader</param>
		/// <param name="vertexSrc">Is the source code from which to compile the shader</param>
		D3D11VertexShader(const std::string_view name, const std::string_view vertexSrc);

		/// <summary>
		/// D3D11 VertexShader Deconstructor
		/// </summary>
		~D3D11VertexShader();

		/// <summary>
		/// Binds the vertex shader to the pipeline
		/// </summary>
		virtual void Bind() const override;

		/// <summary>
		/// Unbinds the vertex shader from the pipeline
		/// </summary>
		virtual void Unbind() const override;

		/// <summary>
		/// Getter for the compiled shader code
		/// </summary>
		/// <returns>The buffer to the compiled shader resource</returns>
		virtual void* GetBufferPointer() const override { return m_pBlob->GetBufferPointer(); }

		/// <summary>
		/// Getter for the compiled shader buffer size
		/// </summary>
		/// <returns>The size of the buffer</returns>
		virtual size_t GetBufferSize() const override { return m_pBlob->GetBufferSize(); }

		/// <summary>
		/// Getter for the shader name
		/// </summary>
		/// <returns>The shader name</returns>
		virtual const std::string_view GetName() const override { return m_Name; }

	private:
		std::string ReadFile(const std::string_view filepath);

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pShader;

		Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;


		std::string m_Name;
	};
}


#endif // _WIN32

#endif // RLR_D3D11VERTEXSHADER