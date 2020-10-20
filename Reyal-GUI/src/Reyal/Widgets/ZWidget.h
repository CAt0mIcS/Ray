#pragma once

#include "RlBase.h"
#include "RlWin.h"

#include <Util/ZRect.h>
#include <Util/ZPoint.h>
#include <Util/ZSize.h>


namespace Zeal::Reyal
{
	class Event;

	class RL_API Widget
	{
	public:

		const Util::ZRect& GetRect() const { return m_Rect; }

		uint32_t GetID() const { return m_ID; }

		bool operator==(_In_ const Widget& other)
		{
			return GetID() == other.GetID();
		}

		void MoveBy(const Util::Point& pos);
		void MoveTo(const Util::Point& pos);

		void ResizeBy(const Util::Size& size);
		void ResizeTo(const Util::Size& size);

	protected:
		Widget(_In_opt_ Widget* parent = nullptr);

		Widget* GetEventReceiver(const Event& e);

	protected:
		uint32_t m_ID;
		Widget* m_Parent;

		Util::ZRect m_Rect;

		std::vector<Widget*> m_Children;
		D2D1::Matrix3x2F m_Matrix;

	private:
		static uint32_t s_NextID;
	};
}



