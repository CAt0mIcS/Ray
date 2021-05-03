#pragma once

#include "RBase.h"
#include "../Core/RComponent.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT Widget : public Component
	{
	public:
		virtual ~Widget() = default;

		Widget& operator=(Widget&& other) = default;
		Widget(Widget&& other) = default;

		/**
		 * @returns Screen space coordinates of the widget
		 */
		Float2 GetTranslation() const;

		/**
		 * @returns Scale in pixels of the widget
		 */
		Float2 GetScale() const;

		/**
		 * @returns Width of the widget in pixels
		 */
		float GetWidth() const;

		/**
		 * @returns Height of the widget in pixels
		 */
		float GetHeight() const;

	protected:
		Widget(Entity entity);
	};
}  // namespace At0::Ray
