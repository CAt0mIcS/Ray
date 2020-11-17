#include "Window.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <RlRender/RendererAPI.h>
#include <RlRender/Renderer3D.h>
//#include <RlRender/Renderer2D.h>

#include "Reyal/Events/Event.h"

#include "WinAPIWindow.h"
#include "OpenGLWindow.h"



namespace At0::Reyal
{
	Ref<Window> Window::Create(const std::string_view name, Widget* parent, bool isMainWindow)
	{
		RL_PROFILE_FUNCTION();

		RL_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::D3D11:		return MakeRef<WinAPIWindow>(name, parent, isMainWindow);
		case RendererAPI::API::OpenGL:		return MakeRef<OpenGLWindow>(name, parent, isMainWindow);
		}

		RL_ASSERT(false, "Failed to create the Window");
		return nullptr;
	}

	Window::Window(const std::string_view name, Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_IsMainWindow(isMainWindow), m_EventQueue{}, m_hWnd(nullptr)
	{
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


