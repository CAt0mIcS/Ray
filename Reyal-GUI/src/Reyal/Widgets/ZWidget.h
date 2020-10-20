#pragma once

#include "RlBase.h"

#include <sal.h>
#include <vector>

#include <Util/ZPoint.h>
#include <Util/ZSize.h>


namespace Zeal::Reyal
{
	class Event;

	class RL_API Widget
	{
	public:
		/// <summary>
		/// Gets the unique name of the current Widget
		/// </summary>
		/// <returns>The name of the widget</returns>
		const std::wstring_view GetName() const { return m_Name; }

		/// <summary>
		/// Searches through all children and finds the one with name
		/// </summary>
		/// <param name="name">Is the child's name</param>
		/// <returns>The child, or nullptr if no child was found</returns>
		Widget* FindChild(const std::wstring_view name);

		/// <summary>
		/// Adds a new child to this control
		/// </summary>
		/// <typeparam name="T">Is any base type of Widget</typeparam>
		/// <param name="child">Is the child to add</param>
		/// <returns>The added child</returns>
		template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, Widget*>>>
		T* AddChild(_In_ Widget* child);
	
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

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the same, false otherwise</returns>
		bool operator==(const Widget& other);

		/// <summary>
		/// Virtual Widget Deconstructor
		/// </summary>
		virtual ~Widget();

	protected:
		/// <summary>
		/// Protected Widget Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the Widget</param>
		/// <param name="parent">Is the parent of this Widget</param>
		Widget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

		/// <summary>
		/// Recursively searches through all children and finds the one which should receive the specified event
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>The Widget which should receive the event</returns>
		Widget* GetEventReceiver(const Event& e);

	protected:
		Widget* m_Parent;
		const std::wstring m_Name;
		std::vector<Widget*> m_Children;
	};
	

	template<typename T, typename>
	inline T* Widget::AddChild(Widget* child)
	{
		return (T*)m_Children.emplace_back(child);
	}
}



