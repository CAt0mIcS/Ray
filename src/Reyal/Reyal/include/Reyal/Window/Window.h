#ifndef RL_WINDOW_H
#define RL_WINDOW_H

#include "RlBase.h"
#include <string>

#include "Reyal/Widgets/ZWidget.h"
#include "Reyal/Core/Queue.h"

#include "Reyal/Input/Mouse.h"
#include "Reyal/Input/Keyboard.h"


namespace At0::Reyal
{
	class Event;
	class Renderer3D;
	//class Renderer2D;

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
		static Ref<Window> Create(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		virtual bool ShouldClose() = 0;

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
		void SetImmediateEventHandler(F&& func) { m_ImmediateEventFn = func; }

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
		virtual void Close() const = 0;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		virtual bool IsOpen() const = 0;
		
		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		bool InitRenderer3D();
		
		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		bool InitRenderer2D();

		/// <summary>
		/// Getter for the Renderer of this Window
		/// </summary>
		/// <returns>The Window Renderer</returns>
		virtual Renderer3D* GetRenderer3D() const override;

		/// <summary>
		/// Checks if this window is the main Window
		/// </summary>
		/// <returns>True if it's the main Window, false otherwise</returns>
		bool IsMainWindow() const { return m_IsMainWindow; }

		/// <summary>
		/// Getter for the Native Window
		/// </summary>
		/// <returns>The Native Window</returns>
		void* GetNativeWindow() const { return m_hWnd; }

		/// <summary>
		/// Window Deconstructor
		/// </summary>
		virtual ~Window() = default;

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
		/// <summary>
		/// Called when any event occurs which needs to be handled immediately (WindowCloseEvent)
		/// <param name="receiver">Is the window which received the immediate event</param>
		/// <param name="e">Is the immediate event</param>
		/// <returns>True if the WindowProc should return 0, false if the WindowProc should break</returns>
		/// </summary>
		EventCallbackFn m_ImmediateEventFn;

		/// <summary>
		/// Is the Queue of messages to process, they will be popped in Reyal::Application and dispatched to the layers
		/// </summary>
		Queue<EventMessage, s_MaxMessagesInQueue> m_EventQueue;

		/// <summary>
		/// Specifies the 3D renderer
		/// </summary>
		Scope<Renderer3D> m_Renderer3D;
		
		///// <summary>
		///// Specifies the 2D renderer
		///// </summary>
		//Scope<Renderer2D> m_Renderer2D;

		/// <summary>
		/// Native Window Handle
		/// </summary>
		void* m_hWnd;

		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size2 m_OldSize;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldPos;

	private:
		/// <summary>
		/// Is true, then if this window closes, the application will exit
		/// </summary>
		bool m_IsMainWindow;
	};
}




#endif // RL_WINDOW_H