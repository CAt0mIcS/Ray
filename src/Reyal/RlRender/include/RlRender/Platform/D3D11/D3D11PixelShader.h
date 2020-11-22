#pragma once

#ifdef _WIN32


#include "RlRBase.h"
#include "D3D11RenderBase.h"

#include "RlRender/PixelShader.h"

#include <wrl.h>


namespace At0::Reyal
{
	class RLR_API D3D11PixelShader : public PixelShader, private D3D11RenderBase
	{
	public:
		/// <summary>
		/// D3D11 PixelShader Constructor
		/// </summary>
		/// <param name="filepath">Is the path to the file containing the ALREADY COMPILED Pixel shader</param>
		D3D11PixelShader(const std::string_view filepath, FileState state);

		/// <summary>
		/// D3D11 PixelShader Constructor
		/// </summary>
		/// <param name="name">Is the name of the Pixel shader</param>
		/// <param name="PixelSrc">Is the source code from which to compile the shader</param>
		D3D11PixelShader(const std::string_view name, const std::string_view PixelSrc);

		/// <summary>
		/// D3D11 PixelShader Deconstructor
		/// </summary>
		~D3D11PixelShader();

		/// <summary>
		/// Binds the Pixel shader to the pipeline
		/// </summary>
		virtual void Bind() const override;

		/// <summary>
		/// Unbinds the Pixel shader from the pipeline
		/// </summary>
		virtual void Unbind() const override;

		/// <summary>
		/// Getter for the shader name
		/// </summary>
		/// <returns>The shader name</returns>
		virtual const std::string_view GetName() const override { return m_Name; }

	private:
		std::string ReadFile(const std::string_view filepath);

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pShader;


		std::string m_Name;
	};
}


#endif // _WIN32
