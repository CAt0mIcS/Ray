#pragma once

#include "NPE/Util/Props.h"

#include <string>
#include <vector>
#include <optional>


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

		/**
		* Enum containing all types of controls
		*/
		enum class Type
		{
			INVALID = 0,
			Node, Button, TextBox
		};

	public:
		/**
		* Getter for the control type
		* 
		* @returns the type of the control
		*/
		Type GetType() const { return m_Type; }

		/**
		* Adds position to current position
		* 
		* @param pos is the position to add
		*/
		void MoveBy(const NPoint& pos);

		/**
		* Moves the window to the new position
		* 
		* @param pos is the new position
		*/
		void MoveTo(const NPoint& pos);

		/**
		* Adds size to the current size
		* 
		* @param size is the size to add
		*/
		void ResizeBy(const NSize& size);

		/**
		* Resizes the window to the new size
		* 
		* @param size is the new size
		*/
		void ResizeTo(const NSize& size);

		/**
		* Getter for current position
		* 
		* @returns the control's position
		*/
		const NPoint& GetPos() const { return m_Pos; }

		/**
		* Getter for current size
		*
		* @returns the control's size
		*/
		const NSize& GetSize() const { return m_Size; }
		
		/**
		* Getter for current color
		*
		* @returns the control's color
		*/
		const NColor& GetColor() const { return m_Color; }

		/**
		* Getter for child windows
		* 
		* @returns all children of the current widget
		*/
		const std::vector<Control*>& GetChildren() const { return m_Children; }

		/**
		* Renders the widget
		*/
		virtual bool Render() const = 0;

		virtual std::optional<std::pair<NPoint, NSize>> CalculateLayout(const NPoint& parentPos, const NSize& parentSize) { return { }; }

		/**
		* Checks if control is in window bounds
		* 
		* @returns true if the control is in the window, false otherwise
		*/
		bool IsInWindow() const;

		/**
		* Virtual destructor of control
		*/
		virtual ~Control() = default;

	protected:
		/**
		* Protected Control constructor
		* 
		* @param renderer is the current 2D renderer
		* @param type is the type of the control
		* @param pos is the control position
		* @param size is the control size
		* @param color is the control color
		*/
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


