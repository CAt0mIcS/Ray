#pragma once

#include "../RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <vector>
#include <wrl.h>

struct ID3D11Buffer;

namespace At0::Ray
{
	class RR_API IndexBuffer : public Bindable, private GraphicsResource
	{
	public:
		IndexBuffer(const std::vector<uint16_t>& indices);
		virtual void Bind() override;
		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};
}