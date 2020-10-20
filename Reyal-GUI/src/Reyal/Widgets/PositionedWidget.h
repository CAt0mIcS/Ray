#pragma once

#include "RlBase.h"
#include "ZWidget.h"


namespace Zeal::Reyal
{
	class Event;
	class Widget;

	class RL_API PositionedWidget : public Widget
	{
	public:
		/// <summary>
		/// Moves the position of the Widget by the specified values
		/// </summary>
		/// <param name="pos">Is the position to add to it's current one</param>
		virtual void MoveBy(const Util::Point& pos) = 0;

		/// <summary>
		/// Moves the position of the Widget to the specified value
		/// </summary>
		/// <param name="pos">Is the new position of this Widget</param>
		virtual void MoveTo(const Util::Point& pos) = 0;

		/// <summary>
		/// Resizes the Widget by the specified value
		/// </summary>
		/// <param name="size">Is the value added to the Widget's current size</param>
		virtual void ResizeBy(const Util::Size& size) = 0;

		/// <summary>
		/// Resizes the Widget to the specified value
		/// </summary>
		/// <param name="size">Is the new size of this Widget</param>
		virtual void ResizeTo(const Util::Size& size) = 0;

	protected:
		/// <summary>
		/// Protected Widget Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the Widget</param>
		/// <param name="parent">Is the parent of this Widget</param>
		PositionedWidget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);
	};
}



