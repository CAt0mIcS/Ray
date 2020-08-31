#pragma once

#include "pch.h"


namespace NPE
{
	struct MousePos
	{
		int x, y;

		MousePos& operator+=(const MousePos& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		MousePos& operator-=(const MousePos& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
	};


	class Mouse
	{
		friend class MainWindow;
	public:
		class MouseEvent
		{
		public:
			enum class MouseEventType
			{
				INVALID = 0,
				LPress, LRelease,
				RPress, RRelease,
				MWheelPress, MWheelRelease,
				WheelUp, WheelDown,
				Enter, Leave,
				Move
			};

			MouseEvent(const MouseEventType type, const Mouse& parent)
				: m_Type(type), m_Pos(parent.GetPos()), m_IsRightPressed(parent.IsRightPressed()), m_IsLeftPressed(parent.IsLeftPressed()), m_IsMiddlePressed(parent.IsMiddlePressed()) {}

			MouseEvent()
				: m_Type(MouseEventType::INVALID), m_Pos{ 0, 0 }, m_IsRightPressed(false), m_IsLeftPressed(false), m_IsMiddlePressed(false) {}

			bool IsLeftPressed() const { return m_IsLeftPressed; }
			bool IsRightPressed() const { return m_IsRightPressed; }
			bool IsMiddlePressed() const { return m_IsMiddlePressed; }
			bool IsValid() const { return m_Type != MouseEventType::INVALID; }

			MousePos GetPos() const { return m_Pos; }

		private:
			MouseEventType m_Type;
			MousePos m_Pos;
			bool m_IsLeftPressed;
			bool m_IsRightPressed;
			bool m_IsMiddlePressed;
		};

	public:
		Mouse();

		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;

		bool IsLeftPressed() const { return m_IsLeftPressed; }
		bool IsRightPressed() const { return m_IsRightPressed; }
		bool IsMiddlePressed() const { return m_IsMiddlePressed; }

		MousePos GetPos() const { return m_Pos; }
		
		void ClearStates() { m_IsLeftPressed = false, m_IsRightPressed = false; m_IsMiddlePressed = false; }

		MouseEvent GetEvent();

	private:
		void OnLButtonDown();
		void OnLButtonUp();
		void OnRButtonDown();
		void OnRButtonUp();
		void OnMButtonDown();
		void OnMButtonUp();

		void OnMouseWheelUp();
		void OnMouseWheelDown();
		void OnMouseWheelDelta(int delte);

		void OnMouseMove(int x, int y);

		void TrimBuffer();

	private:
		MousePos m_Pos;
		bool m_IsLeftPressed;
		bool m_IsRightPressed;
		bool m_IsMiddlePressed;
		std::queue<MouseEvent> m_Queue;
		static constexpr unsigned int m_nQueueSize = 32;
		unsigned int m_WheelDeltaCarry;
	};
}



