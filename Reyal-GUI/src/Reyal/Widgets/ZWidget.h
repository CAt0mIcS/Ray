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
		const std::wstring_view GetName() const { return m_Name; }

		Widget* FindChild(const std::wstring_view name);
		
		bool operator==(const Widget& other);

		template<typename T>
		T* AddChild(_In_ Widget* child);
	
		virtual void MoveBy(const Util::Point& pos) = 0;
		virtual void MoveTo(const Util::Point& pos) = 0;
		virtual void ResizeBy(const Util::Size& size) = 0;
		virtual void ResizeTo(const Util::Size& size) = 0;

		virtual ~Widget();
	protected:
		Widget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

		Widget* GetEventReceiver(const Event& e);

	protected:
		Widget* m_Parent;
		const std::wstring m_Name;
		std::vector<Widget*> m_Children;
	};
	
	template<typename T>
	inline T* Widget::AddChild(Widget* child)
	{
		return (T*)m_Children.emplace_back(child);
	}
}



