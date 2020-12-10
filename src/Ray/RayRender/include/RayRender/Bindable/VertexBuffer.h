#pragma once

#include "RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <vector>


namespace At0::Ray
{
	class RR_API VertexBuffer : public Bindable, private GraphicsResource
	{
	public:
		template<typename V>
		VertexBuffer(const std::vector<V>& vertices);
		virtual void Bind() override;

	private:
		inline void CreateBuffer(const D3D11_BUFFER_DESC& bd, const D3D11_SUBRESOURCE_DATA sd);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		uint32_t m_Strides;
	};
	
	
	
	template<typename V>
	inline VertexBuffer::VertexBuffer(const std::vector<V>& vertices)
		: m_Strides(sizeof(V))
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(V) * vertices.size();
		bd.StructureByteStride = sizeof(V);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = vertices.data();

		// Create separate function to be able to use throw macros without linking client with RlUtil
		CreateBuffer(bd, sd);
	}
}