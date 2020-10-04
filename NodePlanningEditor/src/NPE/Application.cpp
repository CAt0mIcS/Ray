#include "pch.h"
#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/ApplicationEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Exceptions.h"

#include "GUI/Window/FileWindow.h"

/**
* QUESTION:
*	Use strings or wstrings
*/


namespace NPE
{
	Application::Application()
		: m_Actions(*this), m_MousePos{}, m_HandleControls{}, m_Lines{}, m_DrawLines(false), m_NeedsToSave(false), m_Zoom(0)
	{
		NPE_THROW_GFX_EXCEPT(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE), "Failed to initialize Com");

		m_FileHandler.CreateOrLoadSave();

		NPE_LOG("Start of loading scene...\n");
		m_FileHandler.LoadScene(m_Window, m_Lines, m_Zoom);
		NPE_LOG("Finnished loading scene\n");
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
		m_Window.CreateTimer(20000, true);
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
		//Add * to title if the scene was modified
		if (m_NeedsToSave)
			m_Window.SetTitle(L"NodePlanningEditor*");
		else
			m_Window.SetTitle(L"NodePlanningEditor");

		switch (e.GetType())
		{
		case GUI::EventType::MouseMoveEvent:
		{
			return OnMouseMove(watched, (GUI::MouseMoveEvent&)e);
		}
		case GUI::EventType::MouseButtonPressedEvent:
		{
			return OnMouseButtonPressed(watched, (GUI::MouseButtonPressedEvent&)e);
		}
		case GUI::EventType::MouseButtonReleasedEvent:
		{
			return OnMouseButtonReleased(watched, (GUI::MouseButtonReleasedEvent&)e);
		}
		case GUI::EventType::KeyPressedEvent:
		{
			return OnKeyPressed(watched, (GUI::KeyPressedEvent&)e);
		}
		case GUI::EventType::CharEvent:
		{
			return OnChar(watched, (GUI::CharEvent&)e);
		}
		case GUI::EventType::MouseWheelUpEvent:
		{
			return OnMouseWheelUp(watched, (GUI::MouseWheelUpEvent&)e);
		}
		case GUI::EventType::MouseWheelDownEvent:
		{
			return OnMouseWheelDown(watched, (GUI::MouseWheelDownEvent&)e);
		}
		case GUI::EventType::TimerEvent:
		{
			return OnTimer(watched, (GUI::TimerEvent&)e);
		}
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent(watched, (GUI::PaintEvent&)e);
		}
		case GUI::EventType::AppCloseEvent:
		{
			return OnClose(watched, (GUI::AppCloseEvent&)e);
		}
		}
		return false;
	}

	bool Application::OnMouseMove(GUI::Control* watched, GUI::MouseMoveEvent& e)
	{
		if (GUI::Mouse::IsMiddlePressed())
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			
			m_Actions.MoveCamera(GUI::Mouse::GetPos() - m_MousePos, m_Window.GetControls());
			m_MousePos = GUI::Mouse::GetPos();
			m_Window.PostRedraw();

			m_NeedsToSave = true;
			return true;
		}
		else if (GUI::Mouse::IsLeftPressed())
		{
			//Redraw caret when selecting
			if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
			{
				m_Window.PostRedraw();
				return true;
			}
			else if (m_DrawLines)
			{
				m_Window.PostRedraw();
				return true;
			}
			else if (m_HandleControls.draggingNode)
			{
				m_Actions.MoveNodes(m_HandleControls.draggingNode, m_MousePos);
				m_MousePos = GUI::Mouse::GetPos();
				m_Window.PostRedraw();

				m_NeedsToSave = true;
				return true;
			}
		}
		return false;
	}

	bool Application::OnMouseButtonPressed(GUI::Control* watched, GUI::MouseButtonPressedEvent& e)
	{
		m_MousePos = GUI::Mouse::GetPos();
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			if (watched->GetType() == GUI::Control::Type::TextBox)
			{
				m_Window.PostRedraw();
			}
			else if (watched->GetType() == GUI::Control::Type::Button)
			{
				m_Lines.emplace_back((GUI::Button*)watched, nullptr);
				m_DrawLines = true;
				return true;
			}
			else if (watched->GetType() == GUI::Control::Type::Node)
			{
				m_HandleControls.draggingNode = (GUI::Node*)watched;
				return true;
			}
		}
		return false;
	}

	bool Application::OnMouseButtonReleased(GUI::Control* watched, GUI::MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			if (!m_HandleControls.draggingNode && m_DrawLines)
			{
				m_Actions.FinnishLineDrawing(m_Lines, m_Window.GetControls());
				m_Window.PostRedraw();
				m_NeedsToSave = true;
				m_DrawLines = false;
			}

			m_HandleControls.draggingNode = nullptr;
			return true;
		}
		else if (e.GetButton() == GUI::MouseButton::Right)
		{
			m_Actions.EraseLine(m_Lines, m_MousePos);
			m_Window.PostRedraw();
			m_NeedsToSave = true;

			if (GUI::Mouse::IsOnControl(watched) && watched->GetType() == GUI::Control::Type::Node)
			{
				m_Actions.DeleteNode((GUI::Node*)watched, m_Window.GetControls(), m_Lines);
				m_NeedsToSave = true;
				m_Window.PostRedraw();
			}

			return true;
		}
		return false;
	}

	bool Application::OnKeyPressed(GUI::Control* watched, GUI::KeyPressedEvent& e)
	{
		//Redraw caret when moving with arrow keys
		if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
		{
			m_Window.PostRedraw();
			return true;
		}

		//Create new node shortcut
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode(m_Window, s_NodeWidth, s_NodeHeight, m_Zoom);
			m_Window.PostRedraw();
			m_NeedsToSave = true;
			return true;
		}
		//Save to shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed(VK_SHIFT) && GUI::Keyboard::IsKeyPressed('S'))
		{
			m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines, m_Zoom, true);
			m_NeedsToSave = true;
		}
		//Save shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('S'))
		{
			m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines, m_Zoom);
			m_NeedsToSave = false;
		}
		//Load scene from new file shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('O'))
		{
			GUI::FileWindow win;

			GUI::FileWindow::FilterSpecs filterSpecs[] =
			{
				{ L"Database File", L"*.dbs;*.txt" },
				{ L"Any File" , L"*.*" }
			};

			auto result = win.ShowOpenDialog(L"Select file to open", filterSpecs, std::size(filterSpecs));
			
			if (result == L"")
				return false;

			m_FileHandler.ChangeScene(Util::WideCharToMultiByte(result));
			m_FileHandler.LoadScene(m_Window, m_Lines, m_Zoom);
			m_Window.PostRedraw();
		}
		return false;
	}

	bool Application::OnChar(GUI::Control* watched, GUI::CharEvent& e)
	{
		if (m_Window.HasFocus())
		{
			float incr = 30.0f;
			switch (e.GetKeyCode())
			{
			case 'w':
				m_Actions.MoveCamera({ 0.0f, incr }, m_Window.GetControls());
				break;
			case 'a':
				m_Actions.MoveCamera({ incr, 0.0f }, m_Window.GetControls());
				break;
			case 's':
				m_Actions.MoveCamera({ 0.0f, -incr }, m_Window.GetControls());
				break;
			case 'd':
				m_Actions.MoveCamera({ -incr, 0.0f }, m_Window.GetControls());
				break;
			default:
				return false;
			}
			
			m_Window.PostRedraw();
			m_NeedsToSave = true;
			return true;
		}
		//Render caret on key press
		else if (watched->GetType() == GUI::Control::Type::TextBox && watched->HasFocus())
		{
			m_NeedsToSave = true;
			m_Window.PostRedraw();
		}

		return false;
	}

	bool Application::OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e)
	{
		if (watched->GetType() == GUI::Control::Type::TextBox)
		{
			m_Actions.ScrollUp((GUI::TextBox*)watched);
		}
		else
		{
			m_Actions.ZoomIn(m_Zoom, m_Window.GetControls());
			m_NeedsToSave = true;
			m_Window.PostRedraw();
		}
		return true;
	}

	bool Application::OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e)
	{
		if (watched->GetType() == GUI::Control::Type::TextBox)
		{
			m_Actions.ScrollDown((GUI::TextBox*)watched);
		}
		else
		{
			m_Actions.ZoomOut(m_Zoom, m_Window.GetControls());
			m_NeedsToSave = true;
			m_Window.PostRedraw();
		}
		return true;
	}

	bool Application::OnTimer(GUI::Control* watched, GUI::TimerEvent& e)
	{
		switch (e.GetTimer()->GetId())
		{
		case s_TimerAutosaveId:
		{
			//TODO: Implement alert to user that a autosave is comming
			if (m_NeedsToSave)
			{
				m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines, m_Zoom);
				m_NeedsToSave = false;
			}
			return true;
		}
		}

		return false;
	}

	Application::~Application()
	{
		CoUninitialize();
	}

	bool Application::OnPaintEvent(GUI::Control* watched, GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
		renderer.BeginDraw();
		m_Window.Render();
		m_Actions.RenderLines(m_Lines);
		renderer.EndDraw();
		return true;
	}

	bool Application::OnClose(GUI::Control* watched, GUI::AppCloseEvent& e)
	{
		if (m_NeedsToSave)
		{
			int result = MessageBox(m_Window.GetNativeWindow(), L"Save changes to the scene?", L"Unsaved changes", MB_YESNOCANCEL);
			if (result == IDYES)
			{
				m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines, m_Zoom);
			}
			else if (result == IDCANCEL)
			{
				return true;
			}
			else
			{
				NPE_LOG("Unsaved changes discarded due to user input");
			}
		}
		NPE_LOG(" ****** Log finnished for NodePlanningEditor ****** ");
		return false;
	}

}


