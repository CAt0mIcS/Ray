#include "RApplicationEvent.h"

#include <sstream>


namespace At0::Ray
{
	// -------------------------------------------------------------------
	// WindowResizeEvent
	// -------------------------------------------------------------------
	WindowResizeEvent::WindowResizeEvent(const Size2& oldSize, const Size2& newSize)
		: m_OldSize(oldSize), m_NewSize(newSize)
	{

	}

	std::string WindowResizeEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[WindowResizeEvent] New Size: " << m_NewSize;
		return oss.str();
	}

	// -------------------------------------------------------------------
	// WindowMoveEvent
	// -------------------------------------------------------------------
	WindowMoveEvent::WindowMoveEvent(const Point2& oldPos, const Point2& newPos)
		: m_OldPos(oldPos), m_NewPos(newPos)
	{

	}

	std::string WindowMoveEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[WindowMoveEvent] New Posititon: " << m_NewPos;
		return oss.str();
	}

	// -------------------------------------------------------------------
	// WindowCloseEvent
	// -------------------------------------------------------------------
	std::string WindowCloseEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[WindowCloseEvent]";
		return oss.str();
	}

	// -------------------------------------------------------------------
	// PaintEvent
	// -------------------------------------------------------------------
	std::string PaintEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[PaintEvent]";
		return oss.str();
	}
}