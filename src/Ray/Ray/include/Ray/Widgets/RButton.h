#pragma once

#include "../RBase.h"
#include "RWidget.h"

#include "../../RayEvent/include/RayEvent/RayEvent.h"


namespace At0::Ray
{
	class Rectangle;

	// QUESTION: AddPaintEventListener is needed to add listener for paint events?
	class RAY_API Button : public Widget
	{
	public:
		/// <param name="name">Is a application-unique name for the button, doesn't represent the button text.</param>
		/// <param name="parent">Is the button parent. Every button needs to have a valid parent (usually the Window).</param>
		Button(const std::string_view name, Widget& parent);

		void SetText(const std::string_view text) { m_Text = text; }
		std::string_view GetText() const { return m_Text; }

		/// <param name="pos">
		/// Screen coordinates of the top left corner of the widget. 
		/// Screen coordinates start at the top left (0, 0) to the bottom right (window.width, window.height)
		/// </param>
		virtual void Move(const Point2& pos) override { m_Pos = pos; }

		/// <param name="size">New width and height of the widget in pixels.</param>
		virtual void Resize(const Size2& size) override { m_Size = size; }

		/// <returns>The screen coordinates of the top left corner of the widget</returns>
		virtual Point2 GetPos() const override { return m_Pos; }

		/// <returns>Width and height of the widget</returns>
		virtual Size2 GetSize() const override { return m_Size; }

		/// <returns>The renderer of the parent. This function will recursively go up until the Window which has the renderer</returns>
		virtual Renderer3D& GetRenderer3D() const override;

		virtual ~Button();

	private:
		Point2 m_Pos;
		Size2 m_Size;
		std::string m_Text;
	};


	class RAY_API PushButton : public Button
	{
	public:
		/// <param name="name">Is a application-unique name for the button, doesn't represent the button text.</param>
		/// <param name="parent">Is the button parent. Every button needs to have a valid parent (usually the Window).</param>
		PushButton(const std::string_view name, Widget& parent);

		/// <summary>
		/// Draws default button style
		/// The client can create a custom button look by inheriting from PushButton and overriding this draw function.
		/// </summary>
		virtual void Draw() override;

	private:
		Rectangle* m_DrawObject;
	};
}