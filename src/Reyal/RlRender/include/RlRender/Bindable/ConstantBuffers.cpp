#include "rlrpch.h"
#include "ConstantBuffers.h"

#include <RlDebug/Instrumentor.h>

#include "RlRender/Renderer3D.h"
#include "RlRender/Drawable/Drawable.h"


namespace At0::Reyal
{
	TransformConstantBuffer::TransformConstantBuffer(const Renderer3D& renderer, const Drawable& parent)
		: m_Renderer(renderer), m_Parent(parent)
	{
		RL_PROFILE_FUNCTION();
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



