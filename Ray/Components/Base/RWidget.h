#pragma once

#include "../RComponent.h"

#include "../../Core/RMath.h"

#include "../../Events/REventListener.h"
#include "../../Events/REngineEvents.h"


namespace At0::Ray
{
	class Window;

	class RAY_EXPORT Widget : public Component, EventListener<FramebufferResizedEvent>
	{
	public:
		virtual ~Widget() = default;

		Widget& operator=(Widget&& other) = default;
		Widget(Widget&& other) = default;

		/**
		 * @returns Screen space coordinates of the widget
		 */
		Float2 GetTranslation() const { return m_Translation; }

		/**
		 * @returns X-Coordinate in screen space
		 */
		float GetX() const { return m_Translation.x; }

		/**
		 * @returns Y-Coordinate in screen space
		 */
		float GetY() const { return m_Translation.y; }

		/**
		 * @returns Scale in pixels of the widget
		 */
		Float2 GetScale() const { return { GetWidth(), GetHeight() }; }

		/**
		 * @returns Width of the widget in pixels
		 */
		float GetWidth() const { return m_Width; }

		/**
		 * @returns Height of the widget in pixels
		 */
		float GetHeight() const { return m_Height; }

		/**
		 * @param coords Coordinates in screen space
		 */
		void SetTranslation(Float2 coords);

		/**
		 * Adds offset to existing translation
		 */
		void Translate(Float2 offset) { SetTranslation(GetTranslation() + offset); }

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
		 * Adds offset.x and offset.y to width and height respectively
		 */
		void Scale(Float2 offset) { SetScale(GetScale() + offset); }

		/**
		 * @param width Width in pixels
		 */
		void SetWidth(float width);

		/**
		 * @param height Height in pixels
		 */
		void SetHeight(float height);

		/**
		 * @returns Name given to the widget at construction
		 */
		std::string_view GetName() const { return m_Name; }

	protected:
		Widget(Entity entity, Window& window, std::string_view name);

	private:
		virtual void OnEvent(FramebufferResizedEvent& e) override;

	private:
		float m_Width{};
		float m_Height{};
		Float2 m_Translation{};
		std::string m_Name;
		UInt2 m_FramebufferSize{};
	};
}  // namespace At0::Ray
