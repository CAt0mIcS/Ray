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


namespace NPE
{
	Application::Application()
		: m_Actions(*this), m_MousePos{}, m_HandleControls{}, m_Lines{}, m_DrawLines(false), m_NeedsToSave(false), m_Zoom(0)
	{

		auto dirExists = [](const std::string& dirNameIn)
		{
			NPE_LOG("Checking if directory {0} exists...", dirNameIn);

			DWORD ftyp = GetFileAttributesA(dirNameIn.c_str());
			if (ftyp == INVALID_FILE_ATTRIBUTES)
				return false;  //something is wrong with your path!

			if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
				return true;   // this is a directory!

			return false;    // this is not a directory!
		};

		auto fileExists = [](const std::string& fileName)
		{
			struct stat buffer;
			NPE_LOG("Checking if file {0} exists...", fileName);
			return (stat(fileName.c_str(), &buffer) == 0);
		};

		if (!dirExists("saves"))
		{
			NPE_LOG("Directory saves doesn't exist, creating it...");
			if (!CreateDirectory(L"saves", nullptr))
			{
				NPE_THROW_EXCEPT_MSG("Failed to create directory to store save file");
			}
		}
		else
			NPE_LOG("Directory exists");

		bool fileExist = true;
		if (!fileExists("saves/save.dbs"))
		{
			NPE_LOG("Directory saves/save.dbs doesn't exist, creating it...");
			std::ofstream writer("saves/save.dbs");
			writer << "";
			writer.close();
			fileExist = false;
		}
		else
			NPE_LOG("File exists");

		m_FileHandler.CreateDatabase("saves\\save.dbs");

		if (!fileExist)
		{
			m_FileHandler.CreateDefaultTemplate(m_Zoom);
		}

		NPE_LOG("Start of loading scene...");
		m_FileHandler.LoadScene(*this, m_Zoom);
		NPE_LOG("Finnished loading scene");
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
			
			m_Actions.MoveCamera(m_MousePos, m_Window.GetControls());
			m_MousePos = GUI::Mouse::GetPos();
			m_Window.PostRedraw();

			m_NeedsToSave = true;
			return true;
		}
		else if (GUI::Mouse::IsLeftPressed())
		{
			if (m_Lines.size() > 0 && m_DrawLines)
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
			if (watched->GetType() == GUI::Control::Type::Button)
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
			m_Actions.FinnishLineDrawing(m_Lines, m_Window.GetControls());
			m_Window.PostRedraw();
			m_NeedsToSave = true;
			m_DrawLines = false;

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
		//Create new node shortcut
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode(m_Window, s_NodeWidth, s_NodeHeight, m_Zoom);
			m_Window.PostRedraw();
			m_NeedsToSave = true;
			return true;
		}
		//Save shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('S'))
		{
			m_FileHandler.SaveScene(*this, m_Zoom);
			m_NeedsToSave = false;
		}
		return false;
	}

	bool Application::OnChar(GUI::Control* watched, GUI::CharEvent& e)
	{
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
			m_FileHandler.SaveScene(*this, m_Zoom);
			m_NeedsToSave = false;
			return true;
		}
		}

		return false;
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
			int result = MessageBox(m_Window.GetNativeWindow(), L"Save changes to the scene?", L"Unsaved changes", MB_YESNO);
			if (result == IDYES)
			{
				m_FileHandler.SaveScene(*this, m_Zoom);
			}
			else
			{
				NPE_LOG("Unsaved changes discarded due to user input");
			}
		}
		NPE_LOG(" ****** Log finnished for NodePlanningEditor ******");
		return true;
	}

}


