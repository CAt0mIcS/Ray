#pragma once

#include "Control.h"



namespace GUI
{
	class Node : public Control
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
	};
}



