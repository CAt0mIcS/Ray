#include "rpch.h"
#include "Window.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>

#include <RayRender/RendererAPI.h>
#include <RayRender/Renderer3D.h>
//#include <RayRender/Renderer2D.h>

#include "WinAPIWindow.h"
#include "OpenGLWindow.h"



namespace At0::Ray
{
	Ref<Window> Window::Create(const std::string_view name, Widget* parent)
	{
		RAY_PROFILE_FUNCTION();

		RAY_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::API::D3D11:		return MakeRef<WinAPIWindow>(name, parent);
#endif
		case RendererAPI::API::OpenGL:		return MakeRef<OpenGLWindow>(name, parent);
		}

		RAY_ASSERT(false, "Failed to create the Window");
		return nullptr;
	}

	Window::Window(const std::string_view name, Widget* parent, bool isMainWindow)
		: Widget(name, parent), /*m_EventQueue{}, */m_CurrentlyHovering(nullptr)
	{
	}

	void Window::SetHoveringWidget()
	{
		RAY_PROFILE_FUNCTION();

		bool setNew = false;
		static auto generateEvents = [this](Widget* child)
		{
			HoverLeaveEvent e(m_CurrentlyHovering);
			for (auto* pListener : EventDispatcher<HoverLeaveEvent>::Get())
			{
				pListener->OnEvent(m_CurrentlyHovering, e);
			}

			m_CurrentlyHovering = child;

			HoverEnterEvent e2(m_CurrentlyHovering);
			for (auto* pListener : EventDispatcher<HoverEnterEvent>::Get())
			{
				pListener->OnEvent(m_CurrentlyHovering, e2);
			}
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
		RAY_LOG_DEBUG("[Window] {0} destroyed.", GetName());
	}
}

