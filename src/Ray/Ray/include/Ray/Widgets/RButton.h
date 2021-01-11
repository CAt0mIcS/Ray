#pragma once

#include "../RBase.h"
#include "RWidget.h"

#include "../../RayEvent/include/RayEvent/RayEvent.h"


namespace At0::Ray
{
	class RAY_API Button : public Widget
	{
	public:
		/// <param name="name">Is a application-unique name for the button, doesn't represent the button text.</param>
		/// <param name="parent">Is the button parent. Every button needs to have a valid parent (usually the Window).</param>
		Button(const std::string_view name, Widget& parent);

		void SetText(const std::string_view text) { m_Text = text; }
		std::string_view GetText() const { return m_Text; }

		virtual ~Button();

	private:
		std::string m_Text;
	};


	class RAY_API PushButton : public Button
	{
	public:
		/// <param name="name">Is a application-unique name for the button, doesn't represent the button text.</param>
		/// <param name="parent">Is the button parent. Every button needs to have a valid parent (usually the Window).</param>
		PushButton(const std::string_view name, Widget& parent);
		virtual ~PushButton();

		/// <summary>
		/// Draws default button style
		/// The client can create a custom button look by inheriting from PushButton and overriding this draw function.
		/// </summary>
		virtual void Draw() override;

		/// <param name="pos">
		/// Screen coordinates of the top left corner of the widget. 
		/// Screen coordinates start at the top left (0, 0) to the bottom right (window.width, window.height)
		/// </param>
		virtual void Move(const Point2& pos) override;

		/// <param name="size">New width and height of the widget in pixels.</param>
		virtual void Resize(const Size2& size) override;

		/// <returns>The screen coordinates of the top left corner of the widget</returns>
		virtual Point2 GetPos() const override;

		/// <returns>Width and height of the widget</returns>
		virtual Size2 GetSize() const override;
	};
}