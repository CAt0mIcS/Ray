#ifndef RL_APPEVENT_H
#define RL_APPEVENT_H

#include "Reyal/Events/Event.h"

#include <../../RlRender/include/RlRender/TypeDefines.h>
#include <sstream>


namespace At0::Reyal
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const Size2& oldSize, const Size2& newSize)
			: m_OldSize(oldSize), m_NewSize(newSize)
		{

		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[WindowResizeEvent] New Size: " << m_NewSize;
			return oss.str();
		}

		const Size2& GetSize() const { return m_NewSize; }
		const Size2& GetOldSize() const { return m_OldSize; }

		static EventType GetStaticType() { return EventType::WindowResizeEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Size2 m_OldSize;
		Size2 m_NewSize;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(const Point2& oldPos, const Point2& newPos)
			: m_OldPos(oldPos), m_NewPos(newPos)
		{

		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[WindowMoveEvent] New Posititon: " << m_NewPos;
			return oss.str();
		}

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		static EventType GetStaticType() { return EventType::WindowMoveEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Point2 m_OldPos;
		Point2 m_NewPos;
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

		static EventType GetStaticType() { return EventType::SetCursorEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
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

		static EventType GetStaticType() { return EventType::WindowCloseEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
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

		static EventType GetStaticType() { return EventType::PaintEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	private:
	};
}

#endif // RL_APPEVENT_H
