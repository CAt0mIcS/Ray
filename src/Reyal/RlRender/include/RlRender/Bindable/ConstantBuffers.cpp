#include "rlrpch.h"
#include "ConstantBuffers.h"

#include "RlRender/Renderer3D.h"
#include "RlRender/Drawable/Drawable.h"


namespace At0::Reyal
{
	TransformConstantBuffer::TransformConstantBuffer(const Renderer3D& renderer, const Drawable& parent)
		: m_Renderer(renderer), m_Parent(parent)
	{
	}

	void TransformConstantBuffer::Bind()
	{
		m_Vcbuf.Update(
			DirectX::XMMatrixTranspose(
				m_Parent.GetTransformXM() * m_Renderer.GetProjection()
			)
		);
		m_Vcbuf.Bind();
	}
}



