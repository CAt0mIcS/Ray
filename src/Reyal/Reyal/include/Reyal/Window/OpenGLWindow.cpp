#include "rlpch.h"
#include "OpenGLWindow.h"

#include <RlRender/Renderer3D.h>

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/KeyboardEvent.h"



namespace At0::Reyal
{
	OpenGLWindow::OpenGLWindow(const std::string_view name, Widget* parent, bool isMainWindow)
		: Window(name, parent, isMainWindow)
	{
	}
	
	OpenGLWindow::~OpenGLWindow()
	{
	}
	
	void OpenGLWindow::Show()
	{
	}
	
	void OpenGLWindow::SetTitle(const std::string_view title)
	{
	}
	
	bool OpenGLWindow::ShouldClose()
	{
		return false;
	}
}


