#pragma once

#include "Control.h"

#include "HWNDTextBox.h"

#include <d2d1.h>
#include <dwrite.h>

namespace GUI
{
	class Node;
	class MouseButtonPressedEvent;
	class SetCursorEvent;

	class GUI_API TextBox : public Control
	{
		friend class HWNDTextBox;
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
		const std::wstring& GetText() const { return m_Text; }
		
		/**
		* Setter for text displayed in the textbox
		* 
		* @param newText is the new text for the textbox
		* @warning function does not render the text, it just sets the member variable
		* @see TextBox::RenderText()
		*/
		void SetText(const std::wstring& newText) { m_Text = newText; }

		/**
		* Setter for multiline textbox
		* 
		* @param multiline should be true if the textbox should be multiline, false otherwise
		*/
		void SetMultiline(const bool multiline) { m_IsMultiline = multiline; }

		/**
		* Setter for the font family
		*
		* @param family is the new font family
		*/
		void SetFontFamily(const std::wstring& family) { m_FontFamily = family; }

		/**
		* Setter for the font size
		*
		* @param size is the new font size
		*/
		void SetFontSize(const float size) { m_FontSize = size; }

		/**
		* Getter for font size
		* 
		* @returns the current font size of the text
		*/
		float GetFontSize() const { return m_FontSize; }

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
		* Handles cursor changed events
		* 
		* @param e is the cursor event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnSetCursor(SetCursorEvent& e);

		/**
		* Handles left clicked mouse button press, opens the edit window
		* 
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		std::wstring m_Text;
		std::wstring m_FontFamily;

		bool m_IsMultiline;
		bool m_CurrentlySelecting;
		float m_FontSize;
	};
}



