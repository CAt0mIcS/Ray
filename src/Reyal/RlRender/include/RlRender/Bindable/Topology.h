#pragma once

#include "RlRBase.h"

#include "Bindable.h"
#include "RlRender/GraphicsResource.h"


namespace At0::Reyal
{
	class RLR_API Topology : public Bindable, private GraphicsResource
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY type);
		virtual void Bind() override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Type;
	};
}