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


struct UI
{
	GUI::Node* node;
} ui;


namespace NPE
{
	Application::Application()
		: m_Actions(this), m_MousePos{ }, m_HandleControls{}, m_Lines{}, m_DrawLines(false)
	{

		auto dirExists = [](const std::string& dirNameIn)
		{
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
			return (stat(fileName.c_str(), &buffer) == 0);
		};

		if (!dirExists("saves"))
		{
			if (!CreateDirectory(L"saves", nullptr))
			{
				//TODO : throw error
			}
		}

		if (!fileExists("save.dbs"))
		{
			std::ofstream writer("saves/save.dbs");
			writer << "";
			writer.close();
		}

		m_FileHandler.CreateDatabase("saves\\save.dbs");

		if (!fileExists("save.dbs"))
		{
			m_FileHandler.CreateDefaultTemplate(m_Actions.m_Zoom);
		}

		m_FileHandler.LoadScene(*this, m_Actions.m_Zoom);
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
	
		ui.node = m_Window.AddControl<GUI::Node>(new GUI::Node(&m_Window));
		ui.node->SetSize({ 450, 280 });
		ui.node->SetPos({ 250, 180 });
		ui.node->SetColor(GUI::g_DefaultNodeColor);
		ui.node->Init();
	
	}

	bool Application::OnEvent(GUI::Control* watched, GUI::Event& e)
	{
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
		case GUI::EventType::MouseWheelUpEvent:
		{
			return OnMouseWheelUp(watched, (GUI::MouseWheelUpEvent&)e);
		}
		case GUI::EventType::MouseWheelDownEvent:
		{
			return OnMouseWheelDown(watched, (GUI::MouseWheelDownEvent&)e);
		}
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent(watched, (GUI::PaintEvent&)e);
		}
		}
		return false;
	}

	bool Application::OnMouseMove(GUI::Control* watched, GUI::MouseMoveEvent& e)
	{
		if (GUI::Mouse::IsMiddlePressed())
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			m_Actions.MoveCamera();
			return true;
		}
		else if (GUI::Mouse::IsLeftPressed())
		{
			if (m_Lines.size() > 0 && m_DrawLines)
			{
				GUI::Renderer::Get().BeginDraw();
				m_Window.Render();
				m_Actions.RenderLines();
				GUI::Renderer::Get().EndDraw();
				return true;
			}
			else if (m_HandleControls.draggingNode)
			{
				m_Actions.MoveNodes(m_HandleControls.draggingNode);
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
			for (auto* control : m_Window.GetControls())
			{
				if (control->IsInWindow() && control->GetType() == GUI::Control::Type::Node)
				{
					GUI::Button* btn = (GUI::Button*)control->GetChildren()[2];
					if (GUI::Mouse::IsOnControl(btn))
					{
						m_Lines[m_Lines.size() - 1].second = btn;
						break;
					}
				}
			}
			if (m_Lines.size() > 0 && m_Lines[m_Lines.size() - 1].second == nullptr)
			{
				m_Lines.erase(m_Lines.end() - 1);
			}

			m_HandleControls.draggingNode = nullptr;
			m_DrawLines = false;
			return true;
		}
		else if (e.GetButton() == GUI::MouseButton::Right)
		{

			auto linesIntersect = [](const Util::NPoint& p1, const Util::NPoint& p2, const Util::NPoint& q1, const Util::NPoint& q2)
			{
				return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
					* ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
					&&
					(((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
						* ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0);
			};

			for (unsigned int i = 0; i < m_Lines.size(); ++i)
			{
				if (linesIntersect(m_Lines[i].first->GetPos(), m_Lines[i].second->GetPos(), m_MousePos, GUI::Mouse::GetPos()))
				{
					m_Lines.erase(m_Lines.begin() + i);
				}
			}

			GUI::Renderer::Get().BeginDraw();
			m_Window.Render();
			GUI::Renderer::Get().EndDraw();

			return true;
		}
		return false;
	}

	bool Application::OnKeyPressed(GUI::Control* watched, GUI::KeyPressedEvent& e)
	{
		//Create new node shortcut
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode();
			return true;
		}
		//Save shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('S'))
		{
			m_FileHandler.SaveScene(*this, m_Actions.m_Zoom);
		}
		return false;
	}

	bool Application::OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e)
	{
		m_Actions.ZoomIn();
		return true;
	}

	bool Application::OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e)
	{
		m_Actions.ZoomOut();
		return true;
	}

	bool Application::OnPaintEvent(GUI::Control* watched, GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
		renderer.BeginDraw();
		m_Window.Render();

		renderer.EndDraw();
		return true;
	}

}


