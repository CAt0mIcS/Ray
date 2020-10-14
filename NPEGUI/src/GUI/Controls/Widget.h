#pragma once

#include "Control.h"


namespace GUI
{
	class GUI_API Widget : public Control
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="parent"></param>
		Widget(_In_opt_ Control* parent = nullptr);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual bool Render() override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="e"></param>
		/// <returns></returns>
		virtual bool OnEvent(Event& e) override;

		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="F"></typeparam>
		/// <param name="func"></param>
		template<typename F, typename = std::enable_if_t<std::is_invocable<F, Widget*>::value>>
		void SetRenderCallback(F&& func) { m_RenderCallback = func; }

	private:
		std::function<void(Widget*)> m_RenderCallback;
	};
}



