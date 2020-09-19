#pragma once

#include "Control.h"


namespace GUI
{
	class GUI_API Button : public Control
	{
	public:
		/**
		* Button contructor
		* 
		* @param parent is the parent control
		*/
		Button(_In_opt_ Control* parent = nullptr);

		/**
		* Setter for button text
		*
		* @param text is the new button text
		*/
		void SetText(_In_ const std::wstring& text) { m_Text = text; }

		/**
		* Calls the renderer and draws the button
		* 
		* @returns true if the button was rendered (in window), false otherwise
		* @warning Function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() override;
		
		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(_In_ Event& e) override;

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
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize) override;

	private:
		std::wstring m_Text;
	};
}

