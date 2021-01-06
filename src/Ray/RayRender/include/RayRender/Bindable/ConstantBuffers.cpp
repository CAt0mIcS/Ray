#include "RayRender/rlrpch.h"
#include "ConstantBuffers.h"

#include <RayDebug/RInstrumentor.h>

#include "RayRender/Renderer3D.h"
#include "RayRender/Drawable/Drawable.h"

#include <DirectXMath.h>


namespace At0::Ray
{
	TransformConstantBuffer::TransformConstantBuffer(const Renderer3D& renderer, const Drawable& parent)
		: m_Renderer(renderer), m_Parent(parent)
	{
		RAY_PROFILE_FUNCTION();
	}

	void TransformConstantBuffer::Bind()
	{
		const auto model = m_Parent.GetTransform();
		const Transforms tf =
		{
			model.Transpose(),
			(model * m_Renderer.GetCamera() * m_Renderer.GetProjection()).Transpose()
		};
		m_Vcbuf.Update(tf);
		m_Vcbuf.Bind();
	}
}



