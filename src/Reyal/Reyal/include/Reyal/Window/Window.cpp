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

	Ref<Window> Window::Create(const std::string_view name, Widget* parent, bool isMainWindow)
	{
		RL_PROFILE_FUNCTION();

		RL_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::API::D3D11:		return MakeRef<WinAPIWindow>(name, parent, isMainWindow);
#endif
		case RendererAPI::API::OpenGL:		return MakeRef<OpenGLWindow>(name, parent, isMainWindow);
		}

		RL_ASSERT(false, "Failed to create the Window");
		return nullptr;
	}

	Window::Window(const std::string_view name, Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_IsMainWindow(isMainWindow), m_EventQueue{}, m_hWnd(nullptr), m_CurrentlyHovering(nullptr), m_OldPos{}, m_OldSize{}
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

	bool Window::InitRenderer3D()
	{
		RL_PROFILE_FUNCTION();

		if (!m_Renderer3D)
		{
			m_Renderer3D = MakeScope<Renderer3D>();
			m_Renderer3D->Init(m_hWnd);
			return true;
		}
		return false;
	}

	bool Window::InitRenderer2D()
	{
		RL_PROFILE_FUNCTION();

		//if (!m_Renderer2D)
		//{
		//	m_Renderer2D = MakeScope<Renderer2D>();
		//	m_Renderer2D->Init(m_hWnd);
		//  return true;
		//}
		return false;
	}

	Renderer3D* Window::GetRenderer3D() const
	{
		RL_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer3D();
		}
		return m_Renderer3D.get();
	}
}


