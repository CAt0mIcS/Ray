#pragma once

#include "RlRBase.h"

#include "Bindable.h"
#include "RayRender/GraphicsResource.h"


namespace At0::Ray
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