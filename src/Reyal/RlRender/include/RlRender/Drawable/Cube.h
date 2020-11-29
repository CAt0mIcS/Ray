#pragma once

#include "RlRender/RlRBase.h"
#include "Drawable.h"


namespace At0::Reyal
{
	class RLR_API Cube : public Drawable
	{
	public:
		Cube(float size);
		virtual void Update() override;

	private:

	};
}