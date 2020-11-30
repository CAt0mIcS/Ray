#pragma once

#include "RlRender/RlRBase.h"
#include "Drawable.h"


namespace At0::Reyal
{
	class RLR_API Cube : public Drawable
	{
	public:
		Cube(const Renderer3D& renderer, float size);
		virtual void Update() override;
		//virtual DirectX::XMMATRIX GetTransformXM() const override;
	};
}