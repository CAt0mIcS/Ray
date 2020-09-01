#pragma once

#include "pch.h"
#include "NPE/Util/Props.h"


namespace NPE
{

	class Mouse
	{
		friend class MainWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				INVALID = 0,
				LPress, LRelease,
				RPress, RRelease,
				MWheelPress, MWheelRelease,
				WheelUp, WheelDown,
				Enter, Leave,
				Move
			};

			Event(const Type type, const Mouse* parent)
				: m_Type(type), m_Parent(parent) {}

			Event()
				: m_Type(Type::INVALID), m_Parent(nullptr) {}

			bool IsLeftPressed() const { return m_Parent->m_IsLeftPressed; }
			bool IsRightPressed() const { return m_Parent->m_IsRightPressed; }
			bool IsMiddlePressed() const { return m_Parent->m_IsMiddlePressed; }
			bool IsValid() const { return m_Type != Type::INVALID; }

			NPoint GetPos() const { return m_Parent->m_Pos; }
			Type GetType() const { return m_Type; }

		private:
			Type m_Type;
			const Mouse* m_Parent;
		};

	public:
		Mouse();

		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;

		bool IsLeftPressed() const { return m_IsLeftPressed; }
		bool IsRightPressed() const { return m_IsRightPressed; }
		bool IsMiddlePressed() const { return m_IsMiddlePressed; }

		NPoint GetPos() const { return m_Pos; }
		
		void ClearStates() { m_IsLeftPressed = false, m_IsRightPressed = false; m_IsMiddlePressed = false; }

		Event GetEvent();

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
		NPoint m_Pos;
		bool m_IsLeftPressed;
		bool m_IsRightPressed;
		bool m_IsMiddlePressed;
		std::queue<Event> m_Queue;
		static constexpr unsigned int m_nQueueSize = 32;
		int m_WheelDeltaCarry;
	};
}



