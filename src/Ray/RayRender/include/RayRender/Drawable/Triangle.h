#pragma once

#include "RayRender/RlRBase.h"
#include "Drawable.h"

namespace At0::Ray
{
	class RR_API Triangle : public Drawable
	{
	public:
		Triangle();
		virtual void Update() override;

		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const { return s_StaticBinds; };

	private:
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}