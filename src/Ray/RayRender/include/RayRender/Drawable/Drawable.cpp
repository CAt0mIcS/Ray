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

	DirectX::XMMATRIX Drawable::GetTransform() const
	{
		return DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll) *
			DirectX::XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
	}

	void Drawable::Draw(Renderer3D* renderer)
	{
		Bind();
		RAY_MEXPECTS(m_pIndexBuffer != nullptr, "[Drawable::Draw] IndexBuffer was not set!");

		renderer->DrawIndexed(m_pIndexBuffer->GetCount());
	}

	void Drawable::Bind()
	{
		for (auto& bindable : GetStaticBinds())
		{
			bindable->Bind();
		}

		for (auto& bindable : m_Binds)
		{
			bindable->Bind();
		}
	}

	Drawable::Drawable()
		: m_pIndexBuffer(nullptr), m_Binds{}, m_Scale{ 1.0f, 1.0f, 1.0f }
	{
		RAY_PROFILE_FUNCTION();

	}
}


