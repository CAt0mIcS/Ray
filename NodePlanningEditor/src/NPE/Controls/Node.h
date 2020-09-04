#pragma once

#include "Control.h"
#include "Button.h"


namespace NPE
{
	class Node : public Control
	{
	public:
		Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color);

		virtual bool Render() const override;

		template<typename F>
		static void SetOnNodeClickedCallback(F&& func) { m_OnNodeClickedCallback = func; }
		static void OnNodeClickedEventCallback(Node& control) { m_OnNodeClickedCallback(control); }

	private:
		static std::function<void(Node&)> m_OnNodeClickedCallback;
	};
}



