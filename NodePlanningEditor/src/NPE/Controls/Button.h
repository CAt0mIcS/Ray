#pragma once

#include "Control.h"

#include <functional>


namespace NPE
{
	class Button : public Control
	{
	public:
		Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text);

		virtual bool Render() const override;
		
		const std::string& GetText() const { return m_Text; }

		template<typename F>
		static void SetOnButtonClickedCallback(F&& func) { m_OnButtonClickedCallback = func; }
		static void OnButtonClickedEventCallback(Button& control) { m_OnButtonClickedCallback(control); }

	private:


	private:
		std::string m_Text;
		static std::function<void(Button&)> m_OnButtonClickedCallback;

	};
}

