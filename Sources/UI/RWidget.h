#pragma once

#include "RBase.h"
#include "../Core/RComponent.h"


namespace At0::Ray
{
	class RAY_EXPORT Widget : public Component
	{
	public:
		virtual ~Widget() = default;

		Widget& operator=(Widget&& other) = default;
		Widget(Widget&& other) = default;

	protected:
		Widget(Entity& entity);
	};
}  // namespace At0::Ray
