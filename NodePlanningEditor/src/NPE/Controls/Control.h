#pragma once

#include "NPE/Util/Props.h"

#include <string>
#include <vector>

/**
* QUESTION: 
*	Should I have a function in the Control class to draw or should I use Renderer2D::DrawControl(Control& control);?
*/


namespace NPE
{
	class Renderer2D;

	class Control
	{
		friend class Renderer2D;
	public:
		enum class Type
		{
			INVALID = 0,
			Node, Button
		};

	public:
		Type GetType() const { return m_Type; }

		void MoveBy(const NPoint& pos);
		void MoveTo(const NPoint& pos);

		void ResizeBy(const NSize& size);
		void ResizeTo(const NSize& size);

		const NPoint& GetPos() const { return m_Pos; }
		const NSize& GetSize() const { return m_Size; }
		
		const NColor& GetColor() const { return m_Color; }

		const std::vector<Control*>& GetChildren() const { return m_Children; }

		virtual bool Render() const = 0;

		bool IsInWindow() const;

		virtual ~Control() = default;

	protected:
		Control(Renderer2D& renderer, const Type type, const NPoint& pos, const NSize& size, const NColor& color);

	protected:
		Renderer2D& m_Renderer;
		NPoint m_Pos;
		NSize m_Size;
		NColor m_Color;

		std::vector<Control*> m_Children;

	private:
		Type m_Type;
	};
}



