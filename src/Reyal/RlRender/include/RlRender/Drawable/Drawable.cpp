#include "rlrpch.h"
#include "Drawable.h"

#include <RlDebug/RlAssert.h>

#include "RlRender/Bindable/Bindable.h"
#include "RlRender/Bindable/IndexBuffer.h"
#include "RlRender/Renderer3D.h"


namespace At0::Reyal
{
	void Drawable::AddBind(Scope<Bindable> bind)
	{
		RL_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		m_Binds.push_back(std::move(bind));
	}

	void Drawable::AddIndexBuffer(Scope<IndexBuffer> indexBuffer)
	{
		RL_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = indexBuffer.get();
		m_Binds.push_back(std::move(indexBuffer));
	}

	void Drawable::Draw(Renderer3D* renderer)
	{
		for (auto& bindable : m_Binds)
		{
			bindable->Bind();
		}
		renderer->DrawIndexed(m_pIndexBuffer->GetCount());
	}

	Drawable::Drawable()
		: m_pIndexBuffer(nullptr), m_Binds{}, m_TransformMatrix(DirectX::XMMatrixIdentity())
	{

	}
}


