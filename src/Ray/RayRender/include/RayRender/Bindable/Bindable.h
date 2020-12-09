#pragma once

#include "RayRender/RlRBase.h"


namespace At0::Ray
{
	class Bindable
	{
	public:
		virtual void Bind() = 0;
		virtual ~Bindable() = default;
		Bindable& operator=(Bindable&&) noexcept = default;
		Bindable(Bindable&&) noexcept = default;

	protected:
		Bindable() = default;
	};
}