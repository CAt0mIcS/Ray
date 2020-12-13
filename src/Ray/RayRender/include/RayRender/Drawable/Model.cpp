#include "RayRender/rlrpch.h"
#include "Model.h"

#include "../Bindable/IndexBuffer.h"
#include "../Bindable/Topology.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"
#include "../Bindable/VertexBuffer.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
	std::vector<Scope<Bindable>> Model::s_StaticBinds;

	Model::Model(std::string_view filepath)
		: m_Mesh(filepath)
	{

	}

	void Model::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void Model::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void Model::SetIndexFromStatic()
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
}