#include "RayRender/rlrpch.h"
#include "Mesh.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	std::vector<Scope<Bindable>> Mesh::s_StaticBinds;

	void Mesh::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void Mesh::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void Mesh::SetIndexFromStatic()
	{
		RAY_PROFILE_FUNCTION();
		for (Scope<Bindable>& bindable : s_StaticBinds)
		{
			if (const IndexBuffer* p = dynamic_cast<IndexBuffer*>(bindable.get()))
			{
				m_pIndexBuffer = p;
				break;
			}
		}
	}

	void Mesh::Update()
	{

	}

}