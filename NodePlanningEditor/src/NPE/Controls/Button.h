#pragma once

#include "Control.h"

#include <functional>


namespace NPE
{
	class Button : public Control
	{
	public:
		/**
		* Button contructor
		* 
		* @param renderer is the current 2D renderer
		* @param pos is the button position
		* @param size is the button size
		* @param color is the button color
		* @param text is the button text
		*/
		Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text);

		/**
		* Calls the renderer and draws the button
		* 
		* @returns true if the button was rendered (in window), false otherwise
		* @warning Function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() const override;
		
		/**
		* Getter for button text
		* 
		* @returns the button text
		*/
		const std::string& GetText() const { return m_Text; }

		/**
		* Sets a function to be called when any button was clicked
		* 
		* @tparam F is any callable and to an std::function<void(Button&)> castable type
		* @param func is the function to be called
		*/
		template<typename F>
		static void SetOnButtonClickedCallback(F&& func) { m_OnButtonClickedCallback = func; }

		/**
		* Calls the function set by SetOnButtonClickedCallback
		* 
		* @param control is the control which receives the clicked event
		*/
		static void OnButtonClickedEventCallback(Button& control) { m_OnButtonClickedCallback(control); }

	private:
		std::string m_Text;
		static std::function<void(Button&)> m_OnButtonClickedCallback;

	};
}

