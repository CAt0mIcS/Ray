#pragma once

#include "Control.h"


namespace GUI
{
	class Button : public Control
	{
	public:
		/**
		* Button contructor
		* 
		* @param parent is the parent control
		*/
		Button(Control* parent = nullptr);

		/**
		* Setter for button text
		*
		* @param text is the new button text
		*/
		void SetText(const std::wstring& text) { m_Text = text; }

		/**
		* Calls the renderer and draws the button
		* 
		* @returns true if the button was rendered (in window), false otherwise
		* @warning Function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() override;
		
		/**
		* Getter for button text
		* 
		* @returns the button text
		*/
		const std::wstring& GetText() const { return m_Text; }

		/**
		* Calculates the layout of a new button
		*
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the control
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) override;

	private:
		std::wstring m_Text;
	};
}

