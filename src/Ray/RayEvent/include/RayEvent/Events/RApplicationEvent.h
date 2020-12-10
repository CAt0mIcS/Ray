#pragma once

#include "REvent.h"

#include <../../RayRender/include/RayRender/TypeDefines.h>


namespace At0::Ray
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const Size2& oldSize, const Size2& newSize);

		virtual std::string ToString() const override;

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
		WindowMoveEvent(const Point2& oldPos, const Point2& newPos);

		virtual std::string ToString() const override;

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		static EventType GetStaticType() { return EventType::WindowMoveEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Point2 m_OldPos;
		Point2 m_NewPos;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		virtual std::string ToString() const override;

		void AbortWindowClose() { m_WindowCloseAborted = true; }
		void ContinueWindowClose() { m_WindowCloseAborted = false; }
		bool WindowCloseAborted() const { return m_WindowCloseAborted; }

		static EventType GetStaticType() { return EventType::WindowCloseEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		bool m_WindowCloseAborted = true;
	};

	class PaintEvent : public Event
	{
	public:
		PaintEvent() = default;

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::PaintEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};
}

