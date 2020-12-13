#pragma once

#include "../RBase.h"
#include <string>

#include "Ray/Widgets/RWidget.h"

#include "Ray/Input/RMouse.h"
#include "Ray/Input/RKeyboard.h"

#include <../../RayEvent/include/RayEvent/RayEvent.h>


namespace At0::Ray
{
	class Event;

	// QUESTION: New Event System
	class RAY_API Window : public Widget, 
		public EventDispatcher<WindowMoveEvent>,
		public EventDispatcher<WindowResizeEvent>,
		public EventDispatcher<WindowCloseEvent>,
		public EventDispatcher<PaintEvent>,
		public EventDispatcher<MouseMoveEvent>,
		public EventDispatcher<MouseButtonPressedEvent>,
		public EventDispatcher<MouseButtonReleasedEvent>,
		public EventDispatcher<MouseWheelDownEvent>,
		public EventDispatcher<MouseWheelUpEvent>,
		public EventDispatcher<MouseWheelLeftEvent>,
		public EventDispatcher<MouseWheelRightEvent>,
		public EventDispatcher<HoverEnterEvent>,
		public EventDispatcher<HoverLeaveEvent>,
		public EventDispatcher<KeyPressedEvent>,
		public EventDispatcher<KeyReleasedEvent>,
		public EventDispatcher<CharEvent>
	{
	public:
		/// <summary>
		/// Create the Window acording to the RendererAPI::API
		/// </summary>
		/// <param name="name">Is the unique Widget name</param>
		/// <param name="parent">Is a parent Window</param>
		/// <returns>The created Window</returns>
		static Ref<Window> Create(const std::string_view name, const Point2 pos, const Size2 size, Widget* parent = nullptr);

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		virtual std::string GetTitle() const = 0;

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		virtual void SetTitle(const std::string_view title) = 0;

		/// <summary>
		/// Shows the Window
		/// </summary>
		virtual void Show() const = 0;

		/// <summary>
		/// Hides the Window
		/// </summary>
		virtual void Hide() const = 0;

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		virtual void Maximize() const = 0;

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		virtual void Minimize() const = 0;

		/// <summary>
		/// Closes the Window
		/// </summary>
		virtual void Close() = 0;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		virtual bool IsOpen() const = 0;
		
		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(const std::string_view path) = 0;

		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer3D() = 0;
		
		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer2D() = 0;

		/// <summary>
		/// Getter for the Native Window
		/// </summary>
		/// <returns>The Native Window</returns>
		virtual void* GetNativeWindow() const = 0;

		/// <summary>
		/// Window Deconstructor
		/// </summary>
		virtual ~Window();

	public:
		// QUESTIONA: Global Mouse and Keyboard instead of MouseInput& GetMouse() fn? (Can keep it)

		/// <summary>
		/// The mouse holding information about which mouse button is pressed in this window
		/// </summary>
		MouseInput Mouse;

		/// <summary>
		/// The keyboard holding information about which key is pressed in this window
		/// </summary>
		KeyboardInput Keyboard;

	protected:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		Window(const std::string_view name, Widget* parent = nullptr);

		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	protected:

		/// <QUESTIONA>
		/// I could have e.g. m_hWnd in the base class (Window) (as protected) and then I wouldn't have to make the GetNativeWindow() function virtual.
		/// 
		/// Protected members? (Move to protected !)
		/// </QUESTIONA>

		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;
	};
}


