#include "Window.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <RlRender/RendererAPI.h>
#include <RlRender/Renderer3D.h>
//#include <RlRender/Renderer2D.h>

#include "Reyal/Events/MouseEvent.h"

#include "WinAPIWindow.h"
#include "OpenGLWindow.h"



namespace At0::Reyal
{
	EventCallbackFn Window::m_ImmediateEventFn;


	Ref<Window> Window::Create(const std::string_view name, Widget* parent)
	{
		RL_PROFILE_FUNCTION();

		RL_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::API::D3D11:		return MakeRef<WinAPIWindow>(name, parent);
#endif
		case RendererAPI::API::OpenGL:		return MakeRef<OpenGLWindow>(name, parent);
		}

		RL_ASSERT(false, "Failed to create the Window");
		return nullptr;
	}

	Window::Window(const std::string_view name, Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_EventQueue{}, m_CurrentlyHovering(nullptr)
	{
	}

	void Window::SetHoveringWidget()
	{
		RL_PROFILE_FUNCTION();

		bool setNew = false;
		static auto generateEvents = [this](Widget* child)
		{
			//QUESTION: Use the HoverEnter object in receiver and event?

			Scope<HoverLeaveEvent> e = MakeScope<HoverLeaveEvent>(m_CurrentlyHovering);
			m_EventQueue.PushBack({ m_CurrentlyHovering, std::move(e) });

			m_CurrentlyHovering = child;

			Scope<HoverEnterEvent> e2 = MakeScope<HoverEnterEvent>(m_CurrentlyHovering);
			m_EventQueue.PushBack({ m_CurrentlyHovering, std::move(e) });
		};

		for (Scope<Widget>& child : m_Children)
		{
			if (Mouse.IsOnWidget(child) && *m_CurrentlyHovering != child)
			{
				generateEvents(child.get());
				setNew = true;
			}
		}

		if (!setNew && !Mouse.IsOnWidget(m_CurrentlyHovering) && Mouse.IsOnWidget(this))
		{
			generateEvents(this);
		}
		// We can assume that no widget is in focus if the mouse is outside the window rect
		// TODO: Implement IsOnWidget function
		// else if (!setNew && !Mouse.IsOnWidget(this))
		// {
		// 	generateEvents(nullptr);
		// }
	}
	
	Window::~Window()
	{
		RL_LOG_DEBUG("[Window] {0} destroyed.", GetName());
	}
}


