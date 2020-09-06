#pragma once

#include "Control.h"
#include "NPE/Util/Props.h"

#include <functional>



namespace NPE
{
	static NColor g_DefaultNodeColor{ 15.0f, 17.0f, 19.0f };

	class Node : public Control
	{
	public:
		/**
		* Node constructor
		* 
		* @param renderer is the current 2D renderer
		* @param pos is the node position
		* @param size is the node size
		* @param color is the node color
		*/
		Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color);

		/**
		* Calls the renderer and draws the node
		*
		* @returns true if the node was rendered (in window), false otherwise
		* @warning Function does not call BeginDraw/EndDraw
		*/
		virtual bool Render() const override;

		/**
		* Sets a function to be called when any node was clicked
		*
		* @tparam F is any callable and to an std::function<void(Button&)> castable type
		* @param func is the function to be called
		*/
		template<typename F>
		static void SetOnNodeClickedCallback(F&& func) { m_OnNodeClickedCallback = func; }

		/**
		* Calls the function set by SetOnNodeClickedCallback
		*
		* @param control is the control which receives the clicked event
		*/
		static void OnNodeClickedEventCallback(Node& control) { m_OnNodeClickedCallback(control); }

	private:
		static std::function<void(Node&)> m_OnNodeClickedCallback;
	};
}



