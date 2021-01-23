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
		/// <returns>The created Window</returns>
		static Ref<Window> Create(std::string_view name, const Point2& pos, const Size2& size, Widget* parent = nullptr);

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void Update() = 0;

		virtual std::string GetTitle() const = 0;
		virtual void SetTitle(std::string_view title) = 0;

		virtual void Show() const = 0;
		virtual void Hide() const = 0;
		virtual void Maximize() const = 0;
		virtual void Minimize() const = 0;

		virtual void Close() = 0;

		virtual bool IsOpen() const = 0;
		virtual void SetIcon(std::string_view path) = 0;
		virtual void SetVSync(bool enabled) = 0;

		virtual void SetMousePos(const Point2& pos) = 0;
		virtual Point2 GetMousePos() const = 0;

		virtual void EnableCursor() = 0;
		virtual void DisableCursor() = 0;
		virtual bool CursorEnabled() const = 0;

		virtual Ref<Renderer3D> GetRenderer3D() const override;

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
		Window(std::string_view name, Widget* parent = nullptr);

		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	protected:
		/// <summary>
		/// Specifies the widget where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

		Size2 m_OldSize;
		Point2 m_OldPos;

		Ref<Renderer3D> m_Renderer3D;
	};
}


