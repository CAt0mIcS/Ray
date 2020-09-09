#pragma once

#include "Control.h"



namespace GUI
{
	class GUI_API Node : public Control
	{
	public:
		/**
		* Node constructor
		* 
		* @param parent is the parent control
		*/
		Node(Control* parent = nullptr);

		/**
		* Calls the renderer and draws the node
		*
		* @returns true if the node was rendered (in window), false otherwise
		* @warning Function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() override;

		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(Event& e) override;
	};
}



