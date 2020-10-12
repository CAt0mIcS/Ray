#include "pch.h"
#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/ApplicationEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/SceneTab.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Exceptions.h"

#include "GUI/Window/FileWindow.h"

/**
* QUESTION:
*	Use strings or wstrings
*	Is it ok to use "using namespace GUI" in source files
*	How to design a library with include directories (Example QRD) --> Good project file structure (CMake structure?)
*/

#define NPE_DEBUG_DISABLE_AUTOSAVE
//#define NPE_DEBUG_RANDOM_NODES



namespace NPE
{
	Application::Application()
		: m_Actions(*this), m_MousePos{}, m_HandleControls{}, m_Lines{}, m_DrawLines(false), m_NeedsToSave(false), m_Tabs{}
	{
		NPE_THROW_WND_EXCEPT(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

		m_FileHandler.CreateOrLoadSave(m_Window, m_Tabs);

		NPE_LOG("Start of loading scene...\n");
		m_FileHandler.LoadScene(m_Window, m_Lines);

		#ifdef NPE_DEBUG_RANDOM_NODES
			auto& controls = m_Window.GetControls();
			controls.clear();
			m_Lines.clear();

			for (int i = 0; i < 5000; ++i)
			{
				auto* node = m_Window.AddControl<GUI::Node>(new GUI::Node(&m_Window));
				node->SetPos({ (float)rand(), (float)rand() });
				node->SetSize({ s_NodeWidth, s_NodeHeight });
				node->SetColor(Constants::g_DefaultNodeColor);
				m_Actions.CreateNodeTemplate(node);
				((GUI::TextBox*)node->GetChildren()[0])->SetText(L"Hello World!");
				((GUI::TextBox*)node->GetChildren()[1])->SetText(L"Hello World2!");
			}
		#endif

		NPE_LOG("Finnished loading scene\n");
		InstallEventFilter([this](GUI::Control* watched, GUI::Event& e) { return OnEvent(watched, e); });
		m_Window.CreateTimer(20000, true);
		m_Window.SetTitle(L"NodePlanningEditor");
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
		/// <summary>
		/// Remove blue effect when mouse leaves tab
		/// </summary>
		{		
			static bool s_RenderTabs = false;

			if (watched->GetType() == GUI::Control::Type::Tab)
				s_RenderTabs = true;

			if (s_RenderTabs && watched->GetType() != GUI::Control::Type::Tab)
			{
				m_Window.PostRedraw();
				s_RenderTabs = false;
			}
		}

		if (GUI::Mouse::IsMiddlePressed())
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			
			const Util::NPoint diff = GUI::Mouse::GetTransformedPos() - m_MousePos;
			m_Actions.MoveCamera(diff, m_Window.GetControls());
			m_MousePos = GUI::Mouse::GetTransformedPos();
			m_Window.PostRedraw();

			m_NeedsToSave = true;
			return true;
		}
		else if (GUI::Mouse::IsLeftPressed())
		{
			//Redraw caret when selecting
			if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
			{
				watched->GetParent()->PostRedraw();
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
				m_MousePos = GUI::Mouse::GetTransformedPos();
				m_Window.PostRedraw();

				m_NeedsToSave = true;
				return true;
			}
		}
		return false;
	}

	bool Application::OnMouseButtonPressed(GUI::Control* watched, GUI::MouseButtonPressedEvent& e)
	{
		m_MousePos = GUI::Mouse::GetTransformedPos();
		if (e.GetButton() == GUI::MouseButton::Left)
		{
			if (watched->GetType() == GUI::Control::Type::TextBox)
			{
				watched->GetParent()->PostRedraw();
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
			else if (watched->GetType() == GUI::Control::Type::Tab)
			{
				SwitchTab((GUI::SceneTab*)watched);
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

			if (GUI::Mouse::IsOnTransformedControl(watched) && watched->GetType() == GUI::Control::Type::Node)
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
		//Create new Node shortcut
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode(m_Window, s_NodeWidth, s_NodeHeight);
			m_Window.PostRedraw();
			m_NeedsToSave = true;
			return true;
		}
		//Save to shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed(VK_SHIFT) && GUI::Keyboard::IsKeyPressed('S'))
		{
			SaveScene(true);
			return true;
		}
		//Save shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('S'))
		{
			SaveScene();
			return true;
		}
		//Load scene from new file shortcut
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('O'))
		{
			if (m_NeedsToSave)
			{
				int result = PromptSaveChangesMsgBox();
				if (result == IDCANCEL)
					return false;
			}

			std::string result = m_FileHandler.OpenScene(m_Window, m_Lines);
			if (result != "")
			{
				AddNewTab(Util::MultiByteToWideChar(result));
			}
			m_Window.PostRedraw();
			
			return true;
		}
		// Close current tab
		else if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('W'))
		{
			if (m_NeedsToSave)
			{
				int result = PromptSaveChangesMsgBox();
				if (result == IDCANCEL)
					return false;
			}

			CloseTab(GetActiveSceneTab());
			return true;
		}

		//Redraw caret when moving with arrow keys.
		if (watched->HasFocus() && watched->GetType() == GUI::Control::Type::TextBox)
		{
			watched->GetParent()->PostRedraw();
			return true;
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
			watched->GetParent()->PostRedraw();
		}

		return false;
	}

	bool Application::OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e)
	{
		m_Actions.ZoomIn(m_Window.GetControls());
		m_NeedsToSave = true;
		m_Window.PostRedraw();
		return true;
	}

	bool Application::OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e)
	{
		m_Actions.ZoomOut(m_Window.GetControls());
		m_NeedsToSave = true;
		m_Window.PostRedraw();
		return true;
	}

	bool Application::OnTimer(GUI::Control* watched, GUI::TimerEvent& e)
	{
		switch (e.GetTimer()->GetId())
		{
		#ifndef NPE_DEBUG_DISABLE_AUTOSAVE
			case s_TimerAutosaveId:
			{
				//TODO: Implement alert to user that an autosave is comming
				if (m_NeedsToSave)
					m_FileHandler.SaveScene(m_Window.GetControls(), m_Lines);
				return true;
			}
		#endif
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

		// Store current transform to be able to set it again at the end of this function
		D2D1::Matrix3x2F prevTransform = renderer.GetTransform();

		if (watched->GetType() == GUI::Control::Type::Window)
		{
			// Render the entire scene if the window requested a redraw
			renderer.RenderScene();

			D2D1::Matrix3x2F pageTransform = renderer.GetViewMatrix();
			renderer.SetTransform(pageTransform);
		}
		
		watched->Render();
		m_Actions.RenderLines(m_Lines);

		// Restore previous transform
		renderer.SetTransform(prevTransform);
		
		// Render tabs without transform
		if(watched->GetType() == GUI::Control::Type::Tab)
			watched->Render();
		else if (watched->GetType() == GUI::Control::Type::Window)
		{
			for (auto* tab : GetSceneTabs())
			{
				tab->Render();
			}
		}

		renderer.EndDraw();
		return true;
	}

	bool Application::OnClose(GUI::Control* watched, GUI::AppCloseEvent& e)
	{
		// Display a message box if the scene has unsaved changes
		if (m_NeedsToSave)
		{
			int result = PromptSaveChangesMsgBox();
			if (result == IDCANCEL)
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

	int Application::PromptSaveChangesMsgBox()
	{
		int result = MessageBox(m_Window.GetNativeWindow(), L"Save changes to the scene?", L"Unsaved changes", MB_YESNOCANCEL);
		if (result == IDYES)
		{
			SaveScene();
		}
		return result;
	}

	void Application::SaveScene(bool saveToNewLocation)
	{
		auto* active = GetActiveSceneTab();

		if (active)
		{
			auto newFile = Util::MultiByteToWideChar(m_FileHandler.SaveScene(Util::WideCharToMultiByte(active->GetFilePath()), m_Window.GetControls(), m_Lines, saveToNewLocation));
			if (saveToNewLocation)
			{
				active->SetFile(newFile);

				size_t idx = newFile.find_last_of('\\');
				newFile.erase(newFile.begin(), newFile.begin() + idx + 1);

				active->SetText(newFile);
				m_Window.PostRedraw();
			}

			m_NeedsToSave = false;
		}
	}
	
	GUI::SceneTab* Application::GetActiveSceneTab()
	{
		for (auto* tab : GetSceneTabs())
		{
			if (tab->IsActive())
				return tab;
		}
		return nullptr;
	}

	std::vector<GUI::SceneTab*> Application::GetSceneTabs()
	{
		std::vector<GUI::SceneTab*> tabs;

		for (auto* control : m_Window.GetControls())
		{
			if (control->GetType() == GUI::Control::Type::Tab)
				tabs.emplace_back((GUI::SceneTab*)control);
		}
		return tabs;
	}
	
	void Application::AddNewTab(std::wstring filepath)
	{
		//auto* tab = m_Window.AddControl<GUI::SceneTab>(new GUI::SceneTab(&m_Window));
		auto* tab = new GUI::SceneTab(&m_Window);

		auto lastTab = std::find_if(m_Window.GetControls().rbegin(), m_Window.GetControls().rend(), [](GUI::Control* control) { return control->GetType() == GUI::Control::Type::Tab; });

		m_Window.GetControls().insert(lastTab.base(), tab);

		Util::NPoint pos{};
		static constexpr Util::NSize size{ 100.0f, 25.0f };
		for (auto* tb : m_Window.GetControls())
		{
			if (tb->GetType() == GUI::Control::Type::Tab)
			{
				pos.x += tb->GetSize().width;
				((GUI::SceneTab*)tb)->SetActive(false);
			}
		}

		tab->SetPos(pos);
		tab->SetSize(size);
		tab->SetFile(filepath);
		tab->SetFontSize(14);

		size_t idx = filepath.find_last_of('\\');
		filepath.erase(filepath.begin(), filepath.begin() + idx + 1);
		tab->SetText(filepath);
		tab->SetActive(true);
	}
	
	void Application::SwitchTab(GUI::SceneTab* newTab)
	{
		for (auto* tab : GetSceneTabs())
		{
			tab->SetActive(false);
		}
		newTab->SetActive(true);

		m_FileHandler.ChangeScene(Util::WideCharToMultiByte(newTab->GetFilePath()));

		for (unsigned int i = 0; i < m_Window.GetControls().size(); ++i)
		{
			if (m_Window.GetControls()[i]->GetType() != GUI::Control::Type::Tab)
			{
				delete m_Window.GetControls()[i];
				m_Window.GetControls().erase(m_Window.GetControls().begin() + i);
				--i;
			}
		}
		m_Lines.clear();
		m_FileHandler.LoadScene(m_Window, m_Lines);
		
		m_Window.PostRedraw();
	}
	
	void Application::CloseTab(GUI::SceneTab* tab)
	{
		m_FileHandler.RemoveTabFromConfig(tab);
		
		GUI::SceneTab* tabToOpenAfterClosing;
		std::vector<GUI::Control*>::iterator tabIt = std::find(m_Window.GetControls().begin(), m_Window.GetControls().end(), tab);

		// Tab is at the end of the list
		if (tabIt == m_Window.GetControls().end() - 1)
		{
			tabToOpenAfterClosing = (GUI::SceneTab*)*(tabIt - 1);
		}
		// Tab is at the beginning of the list
		else if (tabIt == m_Window.GetControls().begin())
		{
			tabToOpenAfterClosing = (GUI::SceneTab*)*(tabIt + 1);

			auto tabs = GetSceneTabs();
			for (unsigned int i = 1; i < tabs.size(); ++i)
			{
				tabs[i]->MoveBy({ -tab->GetSize().width, 0.0f });
			}

		}
		// Tab is somewhere in the middle
		else
		{
			tabToOpenAfterClosing = (GUI::SceneTab*)*tabIt + 1;

			for (auto it = tabIt + 1; it != m_Window.GetControls().end(); ++it)
			{
				if ((*it)->GetType() == GUI::Control::Type::Tab)
				{
					(*it)->MoveBy({ -tab->GetSize().width, 0.0f });
				}
			}
		}

		delete tab;
		m_Window.GetControls().erase(std::find(m_Window.GetControls().begin(), m_Window.GetControls().end(), tab));

		SwitchTab(tabToOpenAfterClosing);
	}
}


