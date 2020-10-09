#pragma once

#include "Control.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/Addons/TextBox/Caret.h"

/**
* QUESTION:
*	Should I add class declarations like this or just include the header?
*/


namespace GUI
{
	class Node;
	class MouseWheelUpEvent;
	class MouseWheelDownEvent;

	class GUI_API TextBox : public Control
	{
		friend class Caret;
	public:
		/// <summary>
		/// TextBox Constructor
		/// </summary>
		/// <param name="parent">Is the parent Control</param>
		TextBox(_In_opt_ Control* parent = nullptr);

		/// <summary>
		/// Uses the Renderer to draw the TextBox and all it's child controls, 
		/// but does not call BeginDraw() or EndDraw()
		/// </summary>
		/// <returns>True if the Control was rendered, false otherwise</returns>
		virtual bool Render() override;

		/// <summary>
		/// All events of the specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(_In_ Event& e) override;

		/// <summary>
		/// Renders the currently set text
		/// </summary>
		void RenderText();

		/// <summary>
		/// Getter for the current text
		/// </summary>
		/// <returns>The current text</returns>
		const NText& GetText() const { return m_Text; }

		/// <summary>
		/// Getter for the full text
		/// </summary>
		/// <returns>The full text</returns>
		const std::wstring& GetFullText() const { return m_FullText; }

		/// <summary>
		/// Setter for text displayed in the TextBox, use RenderText() to render it
		/// </summary>
		/// <param name="newText">Is the new text the TextBox will display</param>
		void SetText(_In_ const std::wstring& newText);

		/// <summary>
		/// Setter for multiline TextBox
		/// </summary>
		/// <param name="multiline">Is true if the TextBox should support multiline text, false otherwise</param>
		void SetMultiline(_In_ const bool multiline) { m_IsMultiline = multiline; }

		/// <summary>
		/// Checks if TextBox is multiline
		/// </summary>
		/// <returns>True if the TextBox supports multiline text, false otherwise</returns>
		bool IsMultiline() const { return m_IsMultiline; }

		/// <summary>
		/// Setter for font family
		/// </summary>
		/// <param name="family">Is the name of the font family</param>
		void SetFontFamily(_In_ const std::wstring& family) { m_Text.fontFamily = family; }

		/// <summary>
		/// Setter fot font size
		/// </summary>
		/// <param name="size">Is the size of the font</param>
		void SetFontSize(_In_ const float size) { m_Text.fontSize = size; }

		/// <summary>
		/// Getter for font size
		/// </summary>
		/// <returns>The current font size</returns>
		float GetFontSize() const { return m_Text.fontSize; }

		/// <summary>
		/// Calculates the layout of a new TextBox
		/// </summary>
		/// <param name="parentPos">Is the position of the parent Control</param>
		/// <param name="parentSize">Is the size of the parent Control</param>
		/// <returns>The new position and size of the Control</returns>
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize) override;

	private:
		/// <summary>
		/// Handles scrolling up
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnMouseWheelUp(_In_ MouseWheelUpEvent& e);

		/// <summary>
		/// Handles scrolling down
		/// </summary>
		/// <param name="e">Is the received event</param>
		void OnMouseWheelDown(_In_ MouseWheelDownEvent& e);

		/// <summary>
		/// Changes m_Text to only contain the displayed text
		/// </summary>
		void TrimText();

		/// <summary>
		/// Adds a line above the text
		/// </summary>
		void ExtendText();

	private:
		/// <summary>
		/// Structure which holds text information
		/// </summary>
		NText m_Text;

		/// <summary>
		/// Holds the unmodified text
		/// </summary>
		std::wstring m_FullText;

		/// <summary>
		/// Caret in the TextBox
		/// </summary>
		Caret m_Caret;

		/// <summary>
		/// True if multiple lines are allowed, false otherwise. 
		/// </summary>
		bool m_IsMultiline;

		int m_TxtIndexBegin = 0;
		int m_TxtIndexEnd = 0;

	};
}



