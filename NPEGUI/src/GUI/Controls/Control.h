#pragma once

#include "Util/Util.h"

#include <string>
#include <vector>
#include <optional>


/**
* QUESTION: 
*	Should I have a function in the Control class to draw or should I use Renderer::DrawControl(Control& control);?
*/


namespace GUI
{
	class Renderer;

	class Control
	{
		friend class Renderer;
	public:

		/**
		* Enum containing all types of controls
		*/
		enum class Type
		{
			INVALID = 0,
			Node, Button, TextBox, PlainTextEdit
		};

	public:
		/**
		* Setter for position
		*
		* @param pos is the control position
		*/
		void SetPos(const Util::NPoint& pos) { m_Pos = pos; }

		/**
		* Setter for size
		*
		* @param size is the control size
		*/
		void SetSize(const Util::NSize& size) { m_Size = size; }

		/**
		* Setter for color
		*
		* @param color is the control color
		*/
		void SetColor(const Util::NColor& color) { m_Color = color; }

		/**
		* Setter for type
		*
		* @param type is the control type
		*/
		void SetType(Type type) { m_Type = type; }

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
		void MoveBy(const Util::NPoint& pos);

		/**
		* Moves the window to the new position
		* 
		* @param pos is the new position
		*/
		void MoveTo(const Util::NPoint& pos);

		/**
		* Adds size to the current size
		* 
		* @param size is the size to add
		*/
		void ResizeBy(const Util::NSize& size);

		/**
		* Resizes the window to the new size
		* 
		* @param size is the new size
		*/
		void ResizeTo(const Util::NSize& size);

		/**
		* Getter for current position
		* 
		* @returns the control's position
		*/
		const Util::NPoint& GetPos() const { return m_Pos; }

		/**
		* Getter for current size
		*
		* @returns the control's size
		*/
		const Util::NSize& GetSize() const { return m_Size; }
		
		/**
		* Getter for current color
		*
		* @returns the control's color
		*/
		const Util::NColor& GetColor() const { return m_Color; }

		/**
		* Getter for child windows
		* 
		* @returns all children of the current widget
		*/
		const std::vector<Control*>& GetChildren() const { return m_Children; }

		/**
		* Getter for current control id
		* 
		* @returns the control's id
		*/
		unsigned int GetId() const { return m_Id; }

		/**
		* Renders the widget
		*/
		virtual bool Render() = 0;

		/**
		* Checks if the current control overlaps with other
		* 
		* @param other is the other control
		* @param minDst is the minimum distance allowed between nodes
		* @returns true if this controls overlaps with the other one, false otherwise
		*/
		bool OverlapsWith(const Control* other, const Util::NSize& minDst = { 0.0f, 0.0f });

		/**
		* Calculates the layout of a new control
		*
		* @param parentPos is the position of the parent control
		* @param parentSize is the size of the parent control
		* @returns the new position and size of the control
		*/
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize) { return { }; }

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
		Control(Control* parent = nullptr);

	protected:
		Util::NPoint m_Pos;
		Util::NSize m_Size;
		Util::NColor m_Color;

		Control* m_Parent;
		std::vector<Control*> m_Children;

	private:
		unsigned int m_Id;
		Type m_Type;

		static unsigned int m_NextId;
	};
}


