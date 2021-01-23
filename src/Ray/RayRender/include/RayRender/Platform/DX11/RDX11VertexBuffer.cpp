#include "RayRender/RRpch.h"

#ifdef _WIN32
#include "RDX11VertexBuffer.h"
#include "RDX11Shader.h"

#include "../../Core/RVertex.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	DXGI_FORMAT GetDXGIFormat(VertexLayout::ElementType type);

	DX11VertexBuffer::DX11VertexBuffer(const VertexData& data, const Scope<Shader>& vShader)
		: m_Strides(data.Layout().SizeBytes())
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = (uint32_t)data.SizeBytes();
		bd.StructureByteStride = m_Strides;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data.Data();

		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pBuffer));

		SetInputLayout(data.Layout(), (DX11Shader*)vShader.get());
	}

	void DX11VertexBuffer::Bind()
	{
		const UINT offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &m_Strides, &offset);
		GetContext()->IASetInputLayout(m_pLayout.Get());
	}

	void DX11VertexBuffer::SetInputLayout(const VertexLayout& layout, const DX11Shader* vShader)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc(layout.Size());

		for (uint32_t i = 0; i < desc.size(); ++i)
		{
			D3D11_INPUT_ELEMENT_DESC d{};
			d.SemanticName = layout[i].SemanticName();
			d.SemanticIndex = 0;
			d.Format = GetDXGIFormat(layout[i].Type());
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
		const DX11Shader* pDxShader = (DX11Shader*)vShader;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateInputLayout(desc.data(), desc.size(), pDxShader->GetBufferPointer(), pDxShader->GetBufferSize(), &m_pLayout));
	}


	DXGI_FORMAT GetDXGIFormat(VertexLayout::ElementType type)
	{
		switch (type)
		{
		case VertexLayout::Position2D:	return DXGI_FORMAT_R32G32_FLOAT;
		case VertexLayout::Position3D:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexLayout::Texture2D:	return DXGI_FORMAT_R32G32_FLOAT;
		case VertexLayout::Normal:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexLayout::Tangent:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexLayout::Bitangent:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexLayout::Float3Color:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexLayout::Float4Color:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexLayout::BGRAColor:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case VertexLayout::Count:		return DXGI_FORMAT_UNKNOWN;
		}

		RAY_ASSERT(false, "[GetDXGIFormat] Unknown Shader data type (ID={0})", (uint32_t)type);
		return {};
	}
}

#endif // _WIN32


