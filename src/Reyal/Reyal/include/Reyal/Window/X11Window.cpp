#include "rlpch.h"

#ifdef __linux__

#include <RlRender/Renderer3D.h>
#include "Reyal/Events/Event.h"

#include "X11Window.h"

#include <X11/Xlib.h>


namespace At0::Reyal
{
        X11Window::X11Window(const std::string_view name, Widget* parent, bool isMainWindow)
            : Window(name, parent, isMainWindow), m_Display(XOpenDisplay(0))
        {
            m_hWnd = new ::Window(XCreateWindow(
                m_Display, DefaultRootWindow(m_Display),
                 0, 0, 1080, 720, 0, 
                 CopyFromParent, CopyFromParent, CopyFromParent, 0, 0
            ));

            XMapWindow(m_Display, *(::Window*)m_hWnd);
            XFlush(m_Display);
        }

		X11Window::~X11Window()
        {

        }

		std::string X11Window::GetTitle() const
        {
            return "";
        }

		void X11Window::SetTitle(const std::string_view title)
        {

        }

		void X11Window::Show() const
        {

        }

		void X11Window::Hide() const
        {

        }

		void X11Window::Maximize() const
        {

        }

		void X11Window::Minimize() const
        {

        }

		void X11Window::Close() const
        {

        }

		bool X11Window::IsOpen() const
        {
            return false;
        }

		bool X11Window::ShouldClose()
        {
            return false;
        }
}








#endif