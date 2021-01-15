#pragma once

#include "../../RTopology.h"
#include "RDX11GraphicsResources.h"



namespace At0::Ray
{
	class RR_API DX11Topology : public Topology, DX11GraphicsResources
	{
	public:
		DX11Topology(Type type);

		virtual void Bind() override;

	private:
		Type m_Type;
	};
}