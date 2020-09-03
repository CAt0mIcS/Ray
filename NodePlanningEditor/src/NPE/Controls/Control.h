#pragma once

#include "NPE/Util/Props.h"

#include <string>

/**
* QUESTION: 
*	Should I have a function in the Control class to draw or should I use Renderer2D::DrawControl(Control& control);?
*/


namespace NPE
{
	class Control
	{
		friend class Renderer2D;
	public:
		enum class Type
		{
			INVALID = 0,
			Node,
		};

	public:
		Type GetType() const { return m_Type; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		const NPoint& GetPos() const { return m_Pos; }
		const NSize& GetSize() const { return m_Size; }

		bool IsInWindow() const;

		const std::string& GetText() const { return m_Text; }

	protected:
		Control(HWND parent, const Type type, const NPoint& pos, const NSize& size, const std::string& text);

	protected:
		HWND m_hWndParent;
		NPoint m_Pos;
		NSize m_Size;
		std::string m_Text;

	private:
		Type m_Type;
	};
}



