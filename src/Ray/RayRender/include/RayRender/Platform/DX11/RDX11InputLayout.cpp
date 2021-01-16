#include "../../RRpch.h"

#ifdef _WIN32
#include "RDX11InputLayout.h"
#include "RDX11VertexShader.h"

#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	DXGI_FORMAT GetDXGIFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::UInt:		return DXGI_FORMAT_R32_UINT;
		case ShaderDataType::UInt2:		return DXGI_FORMAT_R32G32_UINT;
		case ShaderDataType::UInt3:		return DXGI_FORMAT_R32G32B32_UINT;
		case ShaderDataType::UInt4:		return DXGI_FORMAT_R32G32B32A32_UINT;
			//case ShaderDataType::Bool:		return DXGI_FORMAT_R8_SINT
		}

		RAY_ASSERT(false, "[GetDXGIFormat] Unknown Shader data type (ID={0})", (uint32_t)type);
		return {};
	}



	DX11InputLayout::DX11InputLayout(std::initializer_list<InputElement> inputElements, const VertexShader* vShader)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc(inputElements.size());

		for (uint32_t i = 0; i < inputElements.size(); ++i)
		{
			const InputElement* currInputElement = inputElements.begin() + i;
			D3D11_INPUT_ELEMENT_DESC d{};
			d.SemanticName = currInputElement->SemanticName.c_str();
			d.SemanticIndex = currInputElement->SemanticIndex;
			d.Format = GetDXGIFormat(currInputElement->Type);
			d.InputSlot = 0;
			d.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			d.InstanceDataStepRate = 0;
			desc[i] = d;
		}

		// Cast is safe here because if the DX11InputLayout was chosen then the VertexShader must also be a DX11VertexShader
		// RAY_TODO: RendererAPI could be changed between construction of objects, e.g.
		// --> VertexShader: D3D11
		// --> InputLayout:  OpenGL
		const DX11VertexShader* pDxShader = (DX11VertexShader*)vShader;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateInputLayout(desc.data(), desc.size(), pDxShader->GetBufferPointer(), pDxShader->GetBufferSize(), &m_pLayout));
	}

	void DX11InputLayout::Bind()
	{
		GetContext()->IASetInputLayout(m_pLayout.Get());
	}
}

#endif // _WIN32



