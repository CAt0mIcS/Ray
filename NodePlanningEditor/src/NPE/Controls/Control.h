#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class MainWindow;

	class Control
	{
	public:
		enum class Type
		{
			INVALID = 0,
			Node,
		};

	public:
		unsigned short GetId() const { return m_Id; }
		Type GetType() const { return m_Type; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		NPoint GetPos() const { return m_Pos; }
		NSize GetSize() const { return m_Size; }

		bool IsInWindow() const;

		HWND GetNativeWindow() const { return m_hWnd; }

	protected:
		Control(MainWindow* parent, const Type type, const NPoint& pos, const NSize& size);

	protected:
		HWND m_hWnd;
		HWND m_hWndParent;
		NPoint m_Pos;
		NSize m_Size;
		unsigned short m_Id;

	private:
		static unsigned short m_NextId;
		Type m_Type;
	};

	inline unsigned short Control::m_NextId = 0;
}



