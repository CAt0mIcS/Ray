#pragma once

#include "Event.h"

#include "Reyal/Core/Vec2.h"


namespace Zeal::Reyal
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const Size& size)
			: m_Size(size)
		{

		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[WindowResizeEvent] New Size: " << m_Size;
			return oss.str();
		}

		const Size& GetSize() const { return m_Size; }

		RL_DECLARE_TYPE_FN(WindowResizeEvent)

	private:
		Size m_Size;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(const Point& pos)
			: m_Pos(pos)
		{

		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[WindowMoveEvent] New Posititon: " << m_Pos;
			return oss.str();
		}

		const Point& GetPos() const { return m_Pos; }

		RL_DECLARE_TYPE_FN(WindowMoveEvent)

	private:
		Point m_Pos;
	};

	class SetCursorEvent : public Event
	{
	public:
		SetCursorEvent()
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[SetCursorEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(SetCursorEvent)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[WindowCloseEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(WindowCloseEvent)
	};

	class PaintEvent : public Event
	{
	public:
		PaintEvent()
		{

		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[PaintEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(PaintEvent)
	private:
	};
}
