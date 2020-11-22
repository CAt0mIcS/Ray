#pragma once

#include "Reyal/RlBase.h"
#include <string>

#include "Reyal/Widgets/ZWidget.h"
#include "Reyal/Core/Queue.h"

#include "Reyal/Input/Mouse.h"
#include "Reyal/Input/Keyboard.h"


namespace At0::Reyal
{
	class Event;

	/// <summary>
	/// Struct pushed into the event queue
	/// </summary>
	struct EventMessage
	{
		Widget* receiver;
		Scope<Event> e;
	};

	/// <summary>
	/// Receives all events
	/// <param name="receiver">Is the Widget that received the event</param>
	/// <param name="e">Is the received Event</param>
	/// <returns>True if the event was handled by the client, false if the event should be handled by the Window</returns>
	/// </summary>
	using EventCallbackFn = std::function<bool(Widget*, Event&)>;


	class RL_API Window : public Widget
	{
	protected:
		static constexpr uint8_t s_MaxMessagesInQueue = 16u;

	public:
		/// <summary>
		/// Create the Window acording to the RendererAPI::API
		/// </summary>
		/// <param name="name">Is the unique Widget name</param>
		/// <param name="parent">Is a parent Window</param>
		/// <returns>The created Window</returns>
		static Ref<Window> Create(const std::string_view name, Widget* parent = nullptr);

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void OnUpdate() = 0;

		/// <summary>
		/// Getter for the current event queue
		/// </summary>
		/// <returns>Returns the event queue for this window</returns>
		Queue<EventMessage, s_MaxMessagesInQueue>& GetEventQueue() { return m_EventQueue; }

		/// <summary>
		/// Sets the function which will be called when any event is received which needs to be handled immediately
		/// </summary>
		/// <typeparam name="F">Is any callable, std::function castable type</typeparam>
		/// <param name="func">Is the function to call</param>
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, EventCallbackFn>>>
		static void SetImmediateEventHandler(F&& func) { m_ImmediateEventFn = func; }

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
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		Window(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);

		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	protected:

		/// <QUESTION>
		/// I could have e.g. m_hWnd in the base class (Window) (as protected) and then I wouldn't have to make the GetNativeWindow() function virtual.
		/// </QUESTION>

		/// <summary>
		/// Called when any event occurs which needs to be handled immediately (WindowCloseEvent)
		/// <param name="receiver">Is the window which received the immediate event</param>
		/// <param name="e">Is the immediate event</param>
		/// <returns>True if the WindowProc should return 0, false if the WindowProc should break</returns>
		/// </summary>
		static EventCallbackFn m_ImmediateEventFn;

		/// <summary>
		/// Is the Queue of messages to process, they will be popped in Reyal::Application and dispatched to the layers
		/// </summary>
		Queue<EventMessage, s_MaxMessagesInQueue> m_EventQueue;

		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

	};
}


