#pragma once

#include "Util/Util.h"
#include "GUIBase.h"

#include <string>
#include <vector>
#include <optional>


/**
* QUESTION: 
*	Should I have a function in the Control class to draw or should I use Renderer::DrawControl(Control& control);?
*/


namespace GUI
{
	class Event;

	class GUI_API Control
	{
		friend class Renderer;
	public:

		/// <summary>
		/// Contains all types of Controls
		/// </summary>
		enum class Type
		{
			INVALID = 0,
			Node, Button, TextBox,
			Window
		};

	public:
		/// <summary>
		/// Setter for position
		/// </summary>
		/// <param name="pos">Is the new position</param>
		void SetPos(_In_ const Util::NPoint& pos) { m_Pos = pos; }

		/// <summary>
		/// Setter for size
		/// </summary>
		/// <param name="size">Is the new size</param>
		void SetSize(_In_ const Util::NSize& size) { m_Size = size; }

		/// <summary>
		/// Setter for display color
		/// </summary>
		/// <param name="color">Is the new color</param>
		void SetColor(_In_ const Util::NColor& color) { m_Color = color; }

		/// <summary>
		/// Getter for the Control Type
		/// </summary>
		/// <returns>The Control's type</returns>
		Type GetType() const { return m_Type; }

		/// <summary>
		/// Adds a position to the current one
		/// </summary>
		/// <param name="pos">Is the position to add</param>
		void MoveBy(_In_ const Util::NPoint& pos);

		/// <summary>
		/// Moves window to the new position
		/// </summary>
		/// <param name="pos">Is the new posistion</param>
		void MoveTo(_In_ const Util::NPoint& pos);

		/// <summary>
		/// Adds size to the current one
		/// </summary>
		/// <param name="size">Is the size to add</param>
		void ResizeBy(_In_ const Util::NSize& size);

		/// <summary>
		/// Resizes the Control to the new size
		/// </summary>
		/// <param name="size">Is the new size</param>
		void ResizeTo(_In_ const Util::NSize& size);

		/// <summary>
		/// Getter for the current Control position
		/// </summary>
		/// <returns>The position of the Control</returns>
		const Util::NPoint& GetPos() const { return m_Pos; }

		/// <summary>
		/// Getter for the current Control size
		/// </summary>
		/// <returns>The size of the Control</returns>
		const Util::NSize& GetSize() const { return m_Size; }
		
		/// <summary>
		/// Getter for the current Control color
		/// </summary>
		/// <returns>The color of the Control</returns>
		const Util::NColor& GetColor() const { return m_Color; }

		/// <summary>
		/// Getter for all child Controls
		/// </summary>
		/// <returns>All the children of this Control</returns>
		const std::vector<Control*>& GetChildren() const { return m_Children; }

		/// <summary>
		/// Getter for Control Id
		/// </summary>
		/// <returns>The Control's Id</returns>
		unsigned int GetId() const { return m_Id; }

		/// <summary>
		/// Renders Control and all it's children
		/// </summary>
		/// <returns>True if the Control was rendered(was in window), false otherwise</returns>
		virtual bool Render() = 0;

		/// <summary>
		/// All events of the specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(_In_ Event& e) = 0;

		/// <summary>
		/// Checks if the Control overlapps with other
		/// </summary>
		/// <param name="other">Is the other Control</param>
		/// <param name="minDst">Is the minimum distance allowed between Controls</param>
		/// <returns>True if this Control overlaps with the other one, false otherwise</returns>
		bool OverlapsWith(_In_ const Control* other, _In_opt_ const Util::NSize& minDst = { 0.0f, 0.0f });

		/// <summary>
		/// Calculates the layout of a new Control
		/// </summary>
		/// <param name="parentPos">Is the position of the parent Control</param>
		/// <param name="parentSize">Is the size of the parent Control</param>
		/// <returns>The Control's new position and size</returns>
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize) { return { }; }

		/// <summary>
		/// Checks If Control is in window bounds
		/// </summary>
		/// <returns>True if the Control is in the window, false otherwise</returns>
		bool IsInWindow() const;

		/// <summary>
		/// Virtual Control Destructor
		/// </summary>
		virtual ~Control();

		/// <summary>
		/// Determines which Control should receive the event
		/// </summary>
		/// <param name="e">Is the event received by the window procedure</param>
		/// <returns>The controls which should receive the event</returns>
		Control* GetEventReceiver(_In_ Event& e);

		/// <summary>
		/// Sets new focus to this Control
		/// </summary>
		void SetFocus() { s_Focus = this; }
		
		/// <summary>
		/// Getter for focused Control
		/// </summary>
		/// <returns>The Control with focus</returns>
		_Ret_maybenull_ _Check_return_ Control* GetFocus() const { return s_Focus; }
		
		/// <summary>
		/// Checks if this Control has focus
		/// </summary>
		/// <returns>True if this Control has focus, false otherwise</returns>
		bool HasFocus() { return this == s_Focus; }

		/*
		* QUESTION:
		*	Should i use _Ret_maybenull_ like this (annoying warning!)
		*/

		/// <summary>
		/// Getter for the parent Control
		/// </summary>
		/// <returns>The parent Control, may be null</returns>
		_Ret_maybenull_ _Check_return_ Control* GetParent() const { return m_Parent; }

		/// <summary>
		/// Sets the Control's Id
		/// </summary>
		/// <param name="id">Is the new Id of this Control</param>
		void SetId(unsigned int id) { m_Id = id; }

	protected:
		/// <summary>
		/// Control Constructor
		/// </summary>
		/// <param name="type">Is the type of the Control</param>
		/// <param name="parent">Is the parent of the Control</param>
		Control(_In_ Type type, _In_opt_ Control* parent = nullptr);

	protected:
		/// <summary>
		/// Specifies Control position
		/// </summary>
		Util::NPoint m_Pos;

		/// <summary>
		/// Specifies Control size
		/// </summary>
		Util::NSize m_Size;

		/// <summary>
		/// Specifies Control color
		/// </summary>
		Util::NColor m_Color;

		/// <summary>
		/// Specifies the parent of this Control, could be null
		/// </summary>
		Control* m_Parent;

		/// <summary>
		/// Specifies a list of this Control's children,
		/// their Render() function needs to be called in this Control's Render() function.
		/// There is currently no function to add a child
		/// </summary>
		std::vector<Control*> m_Children;

		/// <summary>
		/// Static pointer to the Control with focus, could be null
		/// </summary>
		static Control* s_Focus;

	private:
		/// <summary>
		/// The Id of this Control
		/// </summary>
		unsigned int m_Id;

		/// <summary>
		/// The type of this Control
		/// </summary>
		Type m_Type;

		/// <summary>
		/// Used to assign m_Id when creating a new Control,
		/// variable is incremented in the Control Constructor
		/// </summary>
		static unsigned int s_NextId;
	};
}


