#pragma once

#include "Event.h"
#include "NPE/Util/Props.h"

namespace NPE
{
	class AppMoveEvent : public Event
	{
	public:
		AppMoveEvent(const NPoint& pos)
			: m_Pos(pos) {}

		const NPoint& GetPos() const { return m_Pos; }

		NPE_DECLARE_TYPE_FN(AppMoveEvent)

	private:
		NPoint m_Pos;
	};

	class AppResizeEvent : public Event
	{
	public:
		AppResizeEvent(const NSize& size)
			: m_Size(size) {}

		const NSize& GetSize() const { return m_Size; }

		NPE_DECLARE_TYPE_FN(AppResizeEvent)

	private:
		NSize m_Size;
	};

	class AppCloseEvent : public Event
	{
	public:
		AppCloseEvent()
		{

		}

		NPE_DECLARE_TYPE_FN(AppCloseEvent)
	};
}
