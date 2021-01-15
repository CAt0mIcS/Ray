#include "RRpch.h"
#include "RTopology.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11Topology.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
	Ref<Topology> Topology::Create(Type type)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[Topology::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11: return MakeRef<DX11Topology>(type);
		case RendererAPI::OpenGL: break;
		}

		return nullptr;
	}
}