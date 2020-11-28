#pragma once

#include "RlRender/RlRBase.h"


namespace At0::Reyal
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