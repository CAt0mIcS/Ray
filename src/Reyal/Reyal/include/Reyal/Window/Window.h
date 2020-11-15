#ifndef RL_WINDOW_H
#define RL_WINDOW_H

#include "WindowsBaseWindow.h"
#include "LinuxBaseWindow.h"

#include "Reyal/Input/Keyboard.h"
#include "Reyal/Input/Mouse.h"

#include "Reyal/Widgets/ZWidget.h"
#include "Reyal/Core/Queue.h"

#include <functional>



namespace At0::Reyal
{
	class Event;
	class Renderer3D;

	/// <summary>
	/// Receives all events
	/// <param name="receiver">Is the Widget that received the event</param>
	/// <param name="e">Is the received Event</param>
	/// <returns>True if the event was handled by the client, false if the event should be handled by the Window</returns>
	/// </summary>
	using EventCallbackFn = std::function<bool(Widget* receiver, Event& e)>;

	/// <summary>
	/// Struct pushed into the event queue
	/// </summary>
	struct EventMessage
	{
		Widget* receiver;
		Scope<Event> e;
	};

	class RL_API Window : public BaseWindow<Window>, public Widget
	{
	private:
		static constexpr uint8_t s_MaxMessagesInQueue = 16u;

	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		Window(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);

		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~Window();

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		virtual int64_t HandleMessage(const WindowMessage& msg) override;

		/// <summary>
		/// Checks if this window is the main Window
		/// </summary>
		/// <returns>True if it's the main Window, false otherwise</returns>
		bool IsMainWindow() const { return m_IsMainWindow; }

		/// <summary>
		/// Getter for the Renderer of this Window
		/// </summary>
		/// <returns>The Window Renderer</returns>
		virtual Renderer3D* GetRenderer3D() const override;

		/// <summary>
		/// Getter for the native window
		/// </summary>
		/// <returns>The native window handle</returns>
		virtual WindowHandle GetNativeWindow() const override;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		bool ShouldClose(int* exitCode = nullptr);

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
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, std::function<bool(Widget*, Event&)>>>>
		void SetImmediateEventHandler(F&& func) { m_ImmediateEvent = func; }

		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		void InitRenderer2D();

		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		void InitRenderer3D();

	private:
		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	public:
		// QUESTION: Ok, public?

		/// <summary>
		/// The keyboard holding information about which key is pressed in this window
		/// </summary>
		KeyboardInput Keyboard;
		
		/// <summary>
		/// The mouse holding information about which mouse button is pressed in this window
		/// </summary>
		MouseInput Mouse;

	private:
		/// <summary>
		/// Is true, then if this window closes, the application will exit
		/// </summary>
		bool m_IsMainWindow;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldWindowPos;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size2 m_OldSize;

		// TODO: Both renderers in here or only one renderer

		///// <summary>
		///// Specifies the 2D renderer
		///// </summary>
		//Scope<Renderer2D> m_Renderer2D;
		
		/// <summary>
		/// Specifies the 3D renderer
		/// </summary>
		Scope<Renderer3D> m_Renderer3D;

		/// <summary>
		/// Is the Queue of messages to process, they will be popped in Reyal::Application and dispatched to the layers
		/// </summary>
		Queue<EventMessage, s_MaxMessagesInQueue> m_EventQueue;

		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

		/// <summary>
		/// Called when any event occurs which needs to be handled immediately (WindowCloseEvent)
		/// <param name="receiver">Is the window which received the immediate event</param>
		/// <param name="e">Is the immediate event</param>
		/// <returns>True if the WindowProc should return 0, false if the WindowProc should break</returns>
		/// </summary>
		EventCallbackFn m_ImmediateEvent;
	};
}

#endif // RL_WINDOW_H