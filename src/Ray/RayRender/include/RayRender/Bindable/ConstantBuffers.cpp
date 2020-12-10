#include "rlrpch.h"
#include "ConstantBuffers.h"

#include <RayDebug/RInstrumentor.h>

#include "RayRender/Renderer3D.h"
#include "RayRender/Drawable/Drawable.h"


namespace At0::Ray
{
	TransformConstantBuffer::TransformConstantBuffer(const Renderer3D& renderer, const Drawable& parent)
		: m_Renderer(renderer), m_Parent(parent)
	{
		RAY_PROFILE_FUNCTION();
	}

	void TransformConstantBuffer::Bind()
	{
		auto& pTransform = m_Parent.GetTransform();
		auto& rTransform = m_Renderer.GetProjection();
		
		m_Vcbuf.Update(
			DirectX::XMMatrixTranspose(
				pTransform * rTransform
			)
		);
		m_Vcbuf.Bind();
	}
}



