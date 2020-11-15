#include "rlpch.h"

#ifdef __linux__

#include <RlUtil/Exception.h>
#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/ApplicationEvent.h"

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <RlRender/Renderer3D.h>


#include "Window.h"


namespace At0::Reyal
{
    Window::Window(const std::wstring_view name, Widget* parent, bool isMainWindow)
        : Widget(name, parent), m_IsMainWindow(isMainWindow), m_OldWindowPos{}, m_CurrentlyHovering(this), m_OldSize{}
    {
        RL_PROFILE_FUNCTION();
    }

    Window::~Window()
    {
        RL_PROFILE_FUNCTION();
        Close();
    }

    int64_t Window::HandleMessage(const WindowMessage& msg)
    {
        return 0;
    }

    Renderer3D* Window::GetRenderer3D() const
    {
        RL_PROFILE_FUNCTION();

        if(GetParent())
        {
            return GetParent()->GetRenderer3D();
        }
        return m_Renderer3D.get();
    }

    WindowHandle Window::GetNativeWindow() const
    {
        return nullptr;
    }

    bool Window::ShouldClose(int* exitCode)
    {
        return false;
    }

    void Window::InitRenderer2D()
    {
        // if(!m_Renderer2D)
        // {
        //     m_Renderer2D = MakeRef<Renderer2D>();
        //     m_Renderer2D->Init(m_hWnd);
        // }
    }

    void Window::InitRenderer3D()
    {
        if (!m_Renderer3D)
		{
			m_Renderer3D = MakeScope<Renderer3D>();
			m_Renderer3D->Init(m_hWnd);
		}
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
}


#endif // __linux__