#include "Ray/Rpch.h"
#include "RWindow.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>

#include <RayRender/RendererAPI.h>
#include <RayRender/Renderer3D.h>
//#include <RayRender/Renderer2D.h>

#include "RWinAPIWindow.h"
#include "ROpenGLWindow.h"



namespace At0::Ray
{
	Ref<Window> Window::Create(std::string_view name, const Point2& pos, const Size2& size, Widget* parent)
	{
		RAY_PROFILE_FUNCTION();

		RAY_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::API::D3D11:		return MakeRef<WinAPIWindow>(name, pos, size, parent);
#endif
		case RendererAPI::API::OpenGL:		return MakeRef<OpenGLWindow>(name, pos, size, parent);
		}

		RAY_ASSERT(false, "Failed to create the Window");
		return nullptr;
	}

	Renderer3D& Window::GetRenderer3D() const
	{
		RAY_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer3D();
		}
		RAY_MEXPECTS(m_Renderer3D.get() != nullptr, "[Window::GetRenderer3D] Renderer was not initialized.");
		return *m_Renderer3D.get();
	}

	Window::Window(std::string_view name, Widget* parent)
		: Widget(name, parent), m_CurrentlyHovering(nullptr), m_OldPos{}, m_OldSize{}
	{
	}

	void Window::SetHoveringWidget()
	{
		RAY_PROFILE_FUNCTION();

		static auto generateEvents = [this](Widget* child)
		{
			HoverLeaveEvent e(*m_CurrentlyHovering);
			for (auto* pListener : EventDispatcher<HoverLeaveEvent>::Get())
			{
				pListener->OnEvent(*m_CurrentlyHovering, e);
			}

			RAY_MEXPECTS(m_CurrentlyHovering != nullptr && *m_CurrentlyHovering != *child, "[Window::SetHoveringWidget] Already hovering widget is new hovering widget.");
			m_CurrentlyHovering = child;

			HoverEnterEvent e2(*m_CurrentlyHovering);
			for (auto* pListener : EventDispatcher<HoverEnterEvent>::Get())
			{
				pListener->OnEvent(*m_CurrentlyHovering, e2);
			}
		};

		if (!m_CurrentlyHovering)
			m_CurrentlyHovering = this;

		for (Scope<Widget>& child : m_Children)
		{
			if (Mouse.IsOnWidget(*child) && *m_CurrentlyHovering != *child)
			{
				generateEvents(child.get());
				return;
			}
		}

		if (*m_CurrentlyHovering != *this && !Mouse.IsOnWidget(*m_CurrentlyHovering))
		{
			generateEvents(this);
		}
	}

	Window::~Window()
	{
		Log::Debug("[Window] '{0}' destroyed.", GetName());
	}
}


