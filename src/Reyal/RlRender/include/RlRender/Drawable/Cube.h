#pragma once

#include "RlRender/RlRBase.h"
#include "Drawable.h"


namespace At0::Reyal
{
	class RLR_API Cube : public Drawable
	{
	public:
		Cube(const Renderer3D& renderer, float size, const float colors[6][3]);
		virtual void Update() override;
	};
}