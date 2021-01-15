#include "../../RRpch.h"
#include "RDX11Topology.h"

#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	D3D11_PRIMITIVE_TOPOLOGY ToDX11Topology(Topology::Type type)
	{
		switch (type)
		{
		case Topology::TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}

		RAY_ASSERT(false, "[ToDX11Topology] Unknown type (ID={0})", (uint32_t)type);
		return {};
	}

	DX11Topology::DX11Topology(Type type)
		: m_Type(type)
	{

	}

	void DX11Topology::Bind()
	{
		GetContext()->IASetPrimitiveTopology(ToDX11Topology(m_Type));
	}
}