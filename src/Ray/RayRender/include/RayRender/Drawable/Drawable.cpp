#include "RayRender/rlrpch.h"
#include "Drawable.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "RayRender/Bindable/Bindable.h"
#include "RayRender/Bindable/IndexBuffer.h"
#include "RayRender/Renderer3D.h"


namespace At0::Ray
{
	entt::registry Drawable::s_Registry;

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

	Drawable::~Drawable()
	{
		s_Registry.destroy(m_Entity);
	}

	DirectX::XMMATRIX Drawable::GetTransform() const
	{
		TransformComponent& transform = GetComponent<TransformComponent>();
		return DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z) *
			DirectX::XMMatrixRotationQuaternion({ transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, transform.Rotation.w }) *
			DirectX::XMMatrixTranslation(transform.Translation.x, transform.Translation.y, transform.Translation.z);
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
		: m_pIndexBuffer(nullptr), m_Binds{}, m_Entity{ s_Registry.create() }
	{
		RAY_PROFILE_FUNCTION();
		AddComponent<TransformComponent>();
	}
}


