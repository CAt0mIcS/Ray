#pragma once

#include "Control.h"


namespace GUI
{
	class Node;

	class TextBox : public Control
	{
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
		* Renders text in member variable
		* 
		* @returns true if the textbox was in the window, false otherwise
		* @see TextBox::SetText()
		*/
		bool RenderText();

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
		* Calculates the layout of a new textbox
		* 
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the layout
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) override;

	private:
		std::wstring m_Text;
	};
}



