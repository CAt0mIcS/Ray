#include "rlrpch.h"
#include "PointLight.h"


namespace At0::Ray
{
	PointLight::PointLight(Renderer3D& renderer, float radius)
		: m_Renderer(renderer), m_Model(renderer, radius)
	{
	}

	void PointLight::Draw()
	{
		m_Model.SetTranslation(pos.x, pos.y, pos.z);
		m_Model.Draw(&m_Renderer);
	}

	void PointLight::Bind()
	{
		PointLightCBuf buff{};
		buff.pos = pos;
		m_cbuf.Update(buff);
		m_cbuf.Bind();
	}
}

