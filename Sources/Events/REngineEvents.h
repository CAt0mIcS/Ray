#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class WindowResizedEvent
	{
	public:
		WindowResizedEvent(Int2 newSize) : m_Size(newSize) {}

		Int2 GetSize() const { return m_Size; }

	private:
		Int2 m_Size;
	};

	class FramebufferResizedEvent
	{
	public:
		FramebufferResizedEvent(Int2 newSize) : m_Size(newSize) {}

		Int2 GetSize() const { return m_Size; }

	private:
		Int2 m_Size;
	};

	class WindowMovedEvent
	{
	public:
		WindowMovedEvent(Int2 newPos) : m_Pos(newPos) {}

		Int2 GetPosition() const { return m_Pos; }

	private:
		Int2 m_Pos;
	};

	class WindowClosedEvent
	{
	};
}  // namespace At0::Ray
