#pragma once

#include "RayRender/RlRBase.h"
#include "RayRender/Bindable/ConstantBuffers.h"
#include "Drawable/SolidSphere.h"

#include <DirectXMath.h>

namespace At0::Ray
{
	class Renderer3D;

	class RR_API PointLight
	{
	public:
		PointLight(Renderer3D& renderer, float radius = 0.5f);
		void Draw();
		void Bind();
		void SetPos(const DirectX::XMFLOAT3 newPos) { pos = newPos; }

	private:
		struct PointLightCBuf
		{
			DirectX::XMFLOAT3 pos;
			float padding;
		};

		DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
		Renderer3D& m_Renderer;
		PixelConstantBuffer<PointLightCBuf> m_cbuf;
		SolidSphere m_Model;
	};
}