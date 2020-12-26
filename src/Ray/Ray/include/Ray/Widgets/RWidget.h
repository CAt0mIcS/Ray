#pragma once

#include "../RBase.h"

#include <vector>
#include <string>

#include <../../RayUtil/include/RayUtil/GlobalDefines.h>

// 4251 class 'std::string' needs to have dll-interface to be used by clients of class 'At0::Ray::Widget'
#pragma warning(disable : 4251)


namespace At0::Ray
{
	class Event;
	class MouseInput;
	class Renderer2D;
	class Renderer3D;

	class RAY_API Widget
	{
	public:
		Widget(Widget&&) noexcept = default;
		
		std::string_view GetName() const { return m_Name; }
		virtual Renderer3D& GetRenderer3D() const = 0;
		Widget* GetParent() const { return m_Parent; }

		/// <summary>
		/// Searches through all children and finds the one with name
		/// </summary>
		/// <param name="name">Is the child's name</param>
		/// <returns>The child, or nullptr if no child was found</returns>
		Widget& FindChildByName(std::string_view name);

		virtual void SetPos(const Point2& pos) = 0;
		virtual void SetSize(const Size2& size) = 0;

		/// <summary>
		/// Adds a new child to this widget
		/// </summary>
		/// <typeparam name="T">Is any derived type of Widget</typeparam>
		/// <typeparam name="...Args">Are any arguments</typeparam>
		/// <param name="name">Is the unique name of the child</param>
		/// <param name="...arg">Are all the arguments to pass to the constructor of T</param>
		/// <returns>The added child</returns>
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_convertible_v<T*, Widget*>>>
		T* AddChild(std::string_view name, Args&&... arg);

		/// <summary>
		/// Requests the specific Widget to be redrawn
		/// </summary>
		void PostRedraw(bool eraseBkgnd = false) const;

		bool operator==(const Widget& other) const;
		bool operator!=(const Widget& other) const;

		virtual ~Widget();

	protected:
		/// <summary>
		/// Protected Widget Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the Widget</param>
		/// <param name="parent">Is the parent of this Widget</param>
		Widget(std::string_view name, Widget* parent = nullptr);

		/// <summary>
		/// Recursively searches through all children and finds the one which should receive the specified event
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <param name="mouse">Is the mouse of the window the widget is in</param>
		/// <returns>The Widget which should receive the event</returns>
		Widget* GetEventReceiver(const Event& e, const MouseInput& mouse);

	protected:
		Widget* m_Parent;
		const std::string m_Name;
		std::vector<Scope<Widget>> m_Children;
	};


	template<typename T, typename... Args, typename>
	inline T* Widget::AddChild(std::string_view name, Args&&... args)
	{
		return (T*)m_Children.emplace_back(MakeScope(name, std::forward<Args>(args)...));
	}
}


