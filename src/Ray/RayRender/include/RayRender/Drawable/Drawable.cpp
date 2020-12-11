#include "RayRender/rlrpch.h"
#include "Drawable.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "RayRender/Bindable/Bindable.h"
#include "RayRender/Bindable/IndexBuffer.h"
#include "RayRender/Renderer3D.h"


namespace At0::Ray
{
	void Drawable::AddBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		m_Binds.push_back(std::move(bind));
	}

	void Drawable::AddIndexBuffer(Scope<IndexBuffer> indexBuffer)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = indexBuffer.get();
		m_Binds.push_back(std::move(indexBuffer));
	}

	void Drawable::Draw(Renderer3D* renderer)
	{
		for (auto& bindable : GetStaticBinds())
		{
			bindable->Bind();
		}

		for (auto& bindable : m_Binds)
		{
			bindable->Bind();
		}
		renderer->DrawIndexed(m_pIndexBuffer->GetCount());
	}

	Drawable::Drawable()
		: m_pIndexBuffer(nullptr), m_Binds{}, m_TransformMatrix(DirectX::XMMatrixIdentity())
	{
		RAY_PROFILE_FUNCTION();

	}
}


