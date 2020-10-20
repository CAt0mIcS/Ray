#pragma once

#include "RlBase.h"
#include "Reyal/Widgets/ZWidget.h"


namespace Zeal::Reyal
{
	class Event;

	class RL_API Layer : public Widget
	{
	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		/// <param name="name">Is the unique name of this Layer</param>
		/// <param name="parent">Is the parent Widget of this Layer</param>
		Layer(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

		/// <summary>
		/// Function which is called every frame
		/// </summary>
		virtual void OnUpdate() { };

		/// <summary>
		/// Receives all events
		/// </summary>
		/// <param name="receiver">Is the Widget which received the event</param>
		/// <param name="e">Is the received event</param>
		virtual void OnEvent(_In_ Widget* receiver, Event& e) { }

	private:
		std::wstring m_Name;
	};
	
}



