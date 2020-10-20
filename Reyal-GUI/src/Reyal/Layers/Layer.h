#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class Widget;
	class Event;

	class RL_API Layer
	{
	public:
		Layer(const std::string_view name);

		virtual void OnUpdate() { };
		virtual void OnEvent(_In_ Widget* receiver, Event& e) { }
	private:
		std::string m_Name;
	};
}



