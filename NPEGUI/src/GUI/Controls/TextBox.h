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
	class MouseButtonPressedEvent;
	class SetCursorEvent;
	class MouseButtonReleasedEvent;
	class KeyPressedEvent;
	class CharEvent;

	class GUI_API TextBox : public Control
	{
		friend class Caret;
	public:
		/**
		* TextBox constructor
		*
		* @param paarent is the parent control
		*/
		TextBox(Control* parent = nullptr);

		/**
		* Calls the renderer and draws the textbox
		*
		* @returns true if the textbox was rendered (in window), false otherwise
		* @warning function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() override;

		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(Event& e) override;

		/**
		* Renders text in member variable
		*
		* @see TextBox::SetText()
		*/
		void RenderText();

		/**
		* Getter for current text in textbox
		*
		* @returns the current text
		*/
		const NText& GetText() const { return m_Text; }

		/**
		* Setter for text displayed in the textbox
		*
		* @param newText is the new text for the textbox
		* @warning function does not render the text, it just sets the member variable
		* @see TextBox::RenderText()
		*/
		void SetText(const std::wstring& newText) { m_Text.text = newText; }

		/**
		* Setter for multiline textbox
		*
		* @param multiline should be true if the textbox should be multiline, false otherwise
		*/
		void SetMultiline(const bool multiline) { m_IsMultiline = multiline; }

		/**
		* Getter for multiline textbox
		* 
		* @returns true if the textbox is multiline, false otherwise
		*/
		bool IsMultiline() const { return m_IsMultiline; }

		/**
		* Setter for the font family
		*
		* @param family is the new font family
		*/
		void SetFontFamily(const std::wstring& family) { m_Text.fontFamily = family; }

		/**
		* Setter for the font size
		*
		* @param size is the new font size
		*/
		void SetFontSize(const float size) { m_Text.fontSize = size; }

		/**
		* Getter for font size
		*
		* @returns the current font size of the text
		*/
		float GetFontSize() const { return m_Text.fontSize; }

		/**
		* Calculates the layout of a new textbox
		*
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the layout
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) override;

	private:
		/**
		* Handles left clicked mouse button press
		*
		* @param e is the received event
		*/
		void OnMouseButtonPressed(MouseButtonPressedEvent& e);

		/**
		* Handles left clicked mouse button release
		*
		* @param e is the received
		*/
		void OnMouseButtonReleased(MouseButtonReleasedEvent& e);

		/**
		* Handles key pressed events
		* 
		* @param e is the received event
		*/
		void OnKeyPressed(KeyPressedEvent& e);

		/**
		* Handles char events
		* 
		* @param e is the received event
		*/
		void OnCharEvent(CharEvent& e);

	private:
		NText m_Text;

		Caret m_Caret;
		bool m_IsMultiline;

	};
}



