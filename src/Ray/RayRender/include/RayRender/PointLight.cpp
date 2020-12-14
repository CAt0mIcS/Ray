#include "rlrpch.h"
#include "PointLight.h"


namespace At0::Ray
{
	PointLight::PointLight(Renderer3D& renderer, float radius)
		: m_Renderer(renderer), m_Model(renderer, radius), m_cbuf(PointLightCBuf{ {20.0f, 20.0f, 20.0f}, 1.0f })
	{
		pos = { 20.0f, 20.0f, 20.0f };
	}

	void PointLight::Draw()
	{
		m_Model.SetTranslation(pos.x, pos.y, pos.z);
		m_Model.Draw(&m_Renderer);
	}

	void PointLight::Bind()
	{
		PointLightCBuf buff;
		buff.pos = pos;
		buff.padding = 1.0f;
		m_cbuf.Update(buff);
		m_cbuf.Bind();
	}
}

