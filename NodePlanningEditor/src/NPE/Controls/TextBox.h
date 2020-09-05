#pragma once

#include "Control.h"

#include <functional>


namespace NPE
{
	class TextBox : public Control
	{
	public:
		/**
		* TextBox constructor
		* 
		* @param renderer is the 2D renderer
		* @param pos is the textbox pos
		* @param size is the textbox size
		* @param color is the textbox background color
		*/
		TextBox(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& startText = "");

		/**
		* Calls the renderer and draws the textbox and text
		*
		* @returns true if the textbox was rendered (in window), false otherwise
		* @warning function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() const override;

		/**
		* Renders text in member variable
		* 
		* @returns true if the textbox was in the window, false otherwise
		* @see TextBox::SetText()
		*/
		bool RenderText() const;

		/**
		* Getter for current text in textbox
		* 
		* @returns the current text
		*/
		const std::string& GetText() const { return m_Text; }
		
		/**
		* Setter for text displayed in the textbox
		* 
		* @param newText is the new text for the textbox
		* @warning function does not render the text, it just sets the member variable
		* @see TextBox::RenderText()
		*/
		void SetText(const std::string& newText) { m_Text = newText; }

		/**
		* Sets a function to be called when any button was clicked
		*
		* @tparam F is any callable and to an std::function<void(TextBox&)> castable type
		* @param func is the function to be called
		*/
		template<typename F>
		static void SetOnTextBoxClickedCallback(F&& func) { m_OnTextBoxClickedCallback = func; }

		/**
		* Calls the function set by SetOnTextBoxClickedCallback
		*
		* @param control is the control which receives the clicked event
		*/
		static void OnTextBoxClickedEventCallback(TextBox& control) { m_OnTextBoxClickedCallback(control); }

	private:
		std::string m_Text;
		static std::function<void(TextBox&)> m_OnTextBoxClickedCallback;
	};
}



