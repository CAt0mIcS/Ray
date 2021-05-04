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
		 * @returns X-Coordinate in screen space
		 */
		float GetX() const;

		/**
		 * @returns Y-Coordinate in screen space
		 */
		float GetY() const;

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

		/**
		 * @param coords Coordinates in screen space
		 */
		void SetTranslation(Float2 coords);

		/**
		 * @param x X-Coordinate in screen space
		 */
		void SetX(float x);

		/**
		 * @param y Y-Coordinate in screen space
		 */
		void SetY(float y);

		/**
		 * @param scale Width and height in pixels
		 */
		void SetScale(Float2 scale);

		/**
		 * @param width Width in pixels
		 */
		void SetWidth(float width);

		/**
		 * @param height Height in pixels
		 */
		void SetHeight(float height);

	protected:
		Widget(Entity entity);
	};
}  // namespace At0::Ray
