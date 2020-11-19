#ifndef RLR_D3D11VERTEXSHADER
#define RLR_D3D11VERTEXSHADER

#ifdef _WIN32


#include "RlRBase.h"
#include "RlRender/VertexShader.h"

#include <wrl.h>


namespace At0::Reyal
{
	class RLR_API D3D11VertexShader : public VertexShader
	{
	public:
		D3D11VertexShader(const std::string_view filepath);

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pShader;
	};
}


#endif // _WIN32

#endif // RLR_D3D11VERTEXSHADER