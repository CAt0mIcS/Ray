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

	DirectX::XMMATRIX Drawable::GetTranslation() const
	{
		DirectX::XMMATRIX translation = DirectX::XMMatrixIdentity();
		translation.r[3].m128_f32[0] = m_TransformMatrix.r[3].m128_f32[0];
		translation.r[3].m128_f32[1] = m_TransformMatrix.r[3].m128_f32[1];
		translation.r[3].m128_f32[2] = m_TransformMatrix.r[3].m128_f32[2];

		return translation;
	}

	DirectX::XMMATRIX Drawable::GetRollPitchYaw() const
	{
		return DirectX::XMMATRIX();
	}

	DirectX::XMMATRIX Drawable::GetScale() const
	{
		DirectX::XMMATRIX scaling = DirectX::XMMatrixIdentity();
		scaling.r[0].m128_f32[0] = m_TransformMatrix.r[0].m128_f32[0];
		scaling.r[1].m128_f32[1] = m_TransformMatrix.r[1].m128_f32[1];
		scaling.r[2].m128_f32[2] = m_TransformMatrix.r[2].m128_f32[2];
		return scaling;
	}

	void Drawable::Draw(Renderer3D* renderer)
	{
		Bind();
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
		: m_pIndexBuffer(nullptr), m_Binds{}, m_TransformMatrix(DirectX::XMMatrixIdentity())
	{
		RAY_PROFILE_FUNCTION();

	}
}


