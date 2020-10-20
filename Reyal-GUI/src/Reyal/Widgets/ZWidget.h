#pragma once

#include "RlBase.h"
#include "RlWin.h"

#include <Util/ZRect.h>


namespace Zeal::Reyal
{
	class Event;

	class RL_API Widget
	{
	public:

		const Util::ZRect& GetRect() const { return m_Rect; }

		uint32_t GetID() const { return m_ID; }

		bool operator==(_In_ const Widget& other)
		{
			return GetID() == other.GetID();
		}

	protected:
		Widget(_In_opt_ Widget* parent = nullptr);

		Widget* GetEventReceiver(const Event& e);

	protected:
		uint32_t m_ID;
		Widget* m_Parent;

		Util::ZRect m_Rect;

		std::vector<Widget*> m_Children;

	private:
		static uint32_t s_NextID;
	};
}



