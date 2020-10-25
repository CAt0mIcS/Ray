#pragma once

#include "RlBase.h"

#include <sal.h>
#include <vector>

#include "Reyal/Core/Vec2.h"


namespace Zeal::Reyal
{
	class Event;
	class WindowRenderer;
	class Mouse;

	class RL_API Widget
	{
	public:
		/// <summary>
		/// Default Widget Move-Constructor
		/// </summary>
		/// <param name=""></param>
		Widget(Widget&&) = default;

		/// <summary>
		/// Gets the unique name of the current Widget
		/// </summary>
		/// <returns>The name of the widget</returns>
		const std::wstring_view GetName() const { return m_Name; }

		/// <summary>
		/// Moves the position of the Widget by the specified values
		/// </summary>
		/// <param name="pos">Is the position to add to it's current one</param>
		void MoveBy(const Point& pos);

		/// <summary>
		/// Moves the position of the Widget to the specified value
		/// </summary>
		/// <param name="pos">Is the new position of this Widget</param>
		void MoveTo(const Point& pos);

		/// <summary>
		/// Resizes the Widget by the specified value
		/// </summary>
		/// <param name="size">Is the value added to the Widget's current size</param>
		void ResizeBy(const Size& size);

		/// <summary>
		/// Resizes the Widget to the specified value
		/// </summary>
		/// <param name="size">Is the new size of this Widget</param>
		void ResizeTo(const Size& size);

		/// <summary>
		/// Gets the current Widget Matrix
		/// </summary>
		/// <returns>The transform matrix of this Widget</returns>
		const D2D1::Matrix3x2F& GetMatrix() const { return m_Matrix; }

		/// <summary>
		/// Getter for the Window Renderer
		/// </summary>
		/// <returns>The Renderer for this window</returns>
		virtual WindowRenderer* GetRenderer() = 0;

		/// <summary>
		/// Getter for the parent of this Widget
		/// </summary>
		/// <returns>The parent Widget</returns>
		Widget* GetParent() const { return m_Parent; }

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
		/// <param name="parent">Is the parent of the child to add</param>
		/// <returns>The added child</returns>
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_convertible_v<T*, Widget*>>>
		T* AddChild(const std::wstring_view name, Args&&... arg);

		/// <summary>
		/// Requests the specific Widget to be redrawn
		/// </summary>
		virtual void PostRedraw() const {  };

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
		Widget* GetEventReceiver(const Event& e, const Mouse& mouse);

	protected:
		Widget* m_Parent;
		const std::wstring m_Name;
		std::vector<Scope<Widget>> m_Children;
		WindowRenderer* m_Renderer;
		D2D1::Matrix3x2F m_Matrix;
	};


	template<typename T, typename... Args, typename>
	inline T* Widget::AddChild(const std::wstring_view name, Args&&... args)
	{
		return (T*)m_Children.emplace_back(MakeScope(name, std::forward<Args>(args)...));
	}
}



