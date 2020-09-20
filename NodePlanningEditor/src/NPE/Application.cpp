#include "pch.h"
#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/ApplicationEvent.h"
#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"


struct UI
{
	GUI::Node* node;
} ui;


namespace NPE
{
	Application::Application()
		: m_FileHandler("saves\\save.dbs"), m_Actions(this), m_MousePos{ }, m_Zoom(0)
	{
		m_FileHandler.LoadScene(this->GetWindow());
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
			return OnMouseMove((GUI::MouseMoveEvent&)e);
		}
		case GUI::EventType::MouseButtonPressedEvent:
		{
			return OnMouseButtonPressed((GUI::MouseButtonPressedEvent&)e);
		}
		case GUI::EventType::KeyPressedEvent:
		{
			return OnKeyPressed((GUI::KeyPressedEvent&)e);
		}
		case GUI::EventType::AppPaintEvent:
		{
			return OnPaintEvent((GUI::PaintEvent&)e);
		}
		}
		return false;
	}

	bool Application::OnMouseMove(GUI::MouseMoveEvent& e)
	{
		if (GUI::Mouse::IsMiddlePressed())
		{
			m_Actions.MoveCamera();
			return true;
		}
		return false;
	}

	bool Application::OnMouseButtonPressed(GUI::MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == GUI::MouseButton::Middle)
		{
			m_MousePos = GUI::Mouse::GetPos();
			return true;
		}
		return false;
	}

	bool Application::OnKeyPressed(GUI::KeyPressedEvent& e)
	{
		if (GUI::Keyboard::IsKeyPressed(VK_CONTROL) && GUI::Keyboard::IsKeyPressed('A'))
		{
			m_Actions.SpawnNode();
			return true;
		}
		return false;
	}

	bool Application::OnPaintEvent(GUI::PaintEvent& e)
	{
		GUI::Renderer& renderer = GUI::Renderer::Get();
		renderer.BeginDraw();
		m_Window.Render();
		renderer.EndDraw();
		return true;
	}

}


