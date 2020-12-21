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
		/// <summary>
		/// Default Widget Move-Constructor
		/// </summary>
		/// <param name=""></param>
		Widget(Widget&&) noexcept = default;

		/// <summary>
		/// Gets the unique name of the current Widget
		/// </summary>
		/// <returns>The name of the widget</returns>
		std::string_view GetName() const { return m_Name; }

		/// <summary>
		/// Getter for the Window Renderer
		/// </summary>
		/// <returns>The Renderer for this window</returns>
		virtual Renderer3D& GetRenderer3D() const = 0;

		/// <summary>
		/// Getter for the native window
		/// </summary>
		/// <returns>The native window handle</returns>
		virtual void* GetNativeWindow() const = 0;

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
		Widget& FindChild(std::string_view name);

		/// <summary>
		/// Adds a new child to this control
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

		/// <summary>
		/// Region which defines commonly used operators
		/// </summarn>
		#pragma region Operators

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the same, false otherwise</returns>
		bool operator==(const Widget& other);

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the same, false otherwise</returns>
		bool operator==(const Scope<Widget>& other);

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the same, false otherwise</returns>
		bool operator==(const Ref<Widget>& other);

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the not same, false otherwise</returns>
		bool operator!=(const Widget& other);

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the not same, false otherwise</returns>
		bool operator!=(const Scope<Widget>& other);

		/// <summary>
		/// Comparison operator
		/// </summary>
		/// <param name="other">Is the Widget to compare to</param>
		/// <returns>True if these two Widgets are the not same, false otherwise</returns>
		bool operator!=(const Ref<Widget>& other);

		#pragma endregion

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
		Widget(std::string_view name, Widget* parent = nullptr);

		/// <summary>
		/// Recursively searches through all children and finds the one which should receive the specified event
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <param name="mouse">Is the mouse of the window the widget is in</param>
		/// <returns>The Widget which should receive the event</returns>
		Widget* GetEventReceiver(const Event& e, const MouseInput& mouse);

	protected:
		/// <summary>
		/// The parent of this Widget, may be null
		/// </summary>
		Widget* m_Parent;

		/// <summary>
		/// The name of this widget, should be unique across all widgets
		/// </summary>
		const std::string m_Name;

		/// <summary>
		/// All the child widgets
		/// </summary>
		std::vector<Scope<Widget>> m_Children;
	};


	template<typename T, typename... Args, typename>
	inline T* Widget::AddChild(std::string_view name, Args&&... args)
	{
		return (T*)m_Children.emplace_back(MakeScope(name, std::forward<Args>(args)...));
	}
}


