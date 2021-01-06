#pragma once

#include "../RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <d3d11.h>
#include <DirectXMath.h>



namespace At0::Ray
{
	class Drawable;

	template<typename C>
	class ConstantBuffer : public Bindable, protected GraphicsResource
	{
	public:
		void Update(const C& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			GetContext()->Map(
				m_pConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr
			);
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext()->Unmap(m_pConstantBuffer.Get(), 0u);
		}

		ConstantBuffer(const C& consts)
		{
			D3D11_BUFFER_DESC bd{};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.ByteWidth = sizeof(consts);
			bd.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = &consts;
			s_pDevice->CreateBuffer(&bd, &sd, m_pConstantBuffer.GetAddressOf());
		}

		ConstantBuffer()
		{
			D3D11_BUFFER_DESC bd{};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.ByteWidth = sizeof(C);
			bd.StructureByteStride = 0;
			s_pDevice->CreateBuffer(&bd, nullptr, m_pConstantBuffer.GetAddressOf());
		}

		virtual ~ConstantBuffer() = default;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	};


	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_pConstantBuffer;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		virtual void Bind() override
		{
			GetContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		}
	};


	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_pConstantBuffer;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind() override
		{
			GetContext()->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
		}
	};


	class Renderer3D;
	class RR_API TransformConstantBuffer : public Bindable
	{
	public:
		TransformConstantBuffer(const Renderer3D& renderer, const Drawable& parent);
		virtual void Bind() override;

	private:
		struct Transforms
		{
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX model;
		};

		const Drawable& m_Parent;
		const Renderer3D& m_Renderer;
		VertexConstantBuffer<Transforms> m_Vcbuf;
	};
}