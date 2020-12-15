#pragma once

#include "../RBase.h"
#include "Ray/Core/RTimestep.h"

namespace At0::Ray
{
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseWheelUpEvent;
	class MouseWheelDownEvent;
	class MouseWheelLeftEvent;
	class MouseWheelRightEvent;
	class MouseMoveEvent;
	class HoverEnterEvent;
	class HoverLeaveEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class CharEvent;
	class WindowResizeEvent;
	class WindowMoveEvent;
	class PaintEvent;
	class WindowCloseEvent;

	class Widget;
	class Window;

	class RAY_API Layer
	{
	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		/// <param name="name">Is the unique name of this Layer</param>
		Layer(std::string_view name);

		/// <summary>
		/// Function which is called every frame
		/// <param name="ts">Is the current time step</param>
		/// </summary>
		virtual void OnUpdate(Timestep ts) { };

		/// <summary>
		/// Short getter for getting the main window
		/// </summary>
		/// <returns>The main windwo</returns>
		Window& GetMainWindow();

		/// <summary>
		/// Short getter for getting the main window
		/// </summary>
		/// <returns>The main windwo</returns>
		const Window& GetMainWindow() const;

		/// <summary>
		/// Virtual Layer Deconstructor
		/// </summary>
		virtual ~Layer();

	private:
		/// <summary>
		/// Is the name of the current Layer, this name should be unique between Layers
		/// </summary>
		std::string m_Name;
	};
	
}


