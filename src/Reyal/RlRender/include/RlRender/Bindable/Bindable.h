#pragma once

#include "RlRender/RlRBase.h"


namespace At0::Reyal
{
	class Bindable
	{
	public:
		virtual void Bind() = 0;
		Bindable& operator=(Bindable&&) noexcept = default;
		Bindable(Bindable&&) noexcept = default;

	protected:
		Bindable() = default;
	};
}