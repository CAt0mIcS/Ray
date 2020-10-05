#pragma once

#include "Control.h"



namespace GUI
{
	class GUI_API Node : public Control
	{
	public:
		/// <summary>
		/// Node Constructor
		/// </summary>
		/// <param name="parent">Is the parent of the Node</param>
		Node(_In_opt_ Control* parent = nullptr);

		/// <summary>
		/// Creates Node template
		/// </summary>
		void Init();

		/// <summary>
		/// Uses the Renderer to draw the Node and all it's child controls, 
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
	};
}



