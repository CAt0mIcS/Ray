#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Controls/Node.h"
#include "NPE/Controls/Button.h"
#include "NPE/Controls/TextBox.h"

#include "NPE/Util/Debug/Timer.h"


namespace NPE
{
	Application::Application()
		: m_Database("saves\\save.dbs", 2), m_Window({ 800, 600 }, L"NodePlanningEditor", [this](const Event& e) { OnEvent(e); }), m_Zoom(1.0f), m_MousePos{ 0, 0 }
	{

		/* Table fetch */
		//QRD::Table& tbNodeInfo = m_Database.GetTable("NodeInfo");
		//QRD::Table& tbSceneInfo = m_Database.GetTable("SceneInfo");

		//m_Zoom = std::stof(tbSceneInfo.GetRecords()[0].GetRecordData()[0]);

		for (int i = 0; i < 1000; ++i)
		{
			//m_Window.AddControl(new Node(m_Window.Renderer2D, { (float)(rand() / 2), (float)(rand() / 2) }, { 200, 150 }, { 15.0f, 17.0f, 19.0f }));
			m_Window.AddControl(new Node(m_Window.Renderer2D, { (float)(rand() / 2), (float)(rand() / 2) }, { 450, 280}, { 15.0f, 17.0f, 19.0f }));
		}
		//auto* txtbox = (TextBox*)m_Window.GetControls()[m_Window.GetControls().size() - 1];
		//txtbox->SetText(L"Hello world");

		//for (auto& record : tbNodeInfo.GetRecords())
		//{
			//auto& data = record.GetRecordData();
			//m_Window.AddControl(new Node(m_Window.Renderer2D, { std::stof(data[0]), std::stof(data[1]) }, { std::stof(data[2]), std::stof(data[3]) }, { 15.0f, 17.0f, 19.0f }));
		//}

		//clear save file
		//m_Database.DeleteTable("NodeInfo");
		//m_Database.DeleteTable("SceneInfo");
		//m_Database.WriteDb();
		
		/* Table creation and setup */
		//QRD::Table& tbNodeInfoC = m_Database.CreateTable("NodeInfo");
		//QRD::Table& tbSceneInfoC = m_Database.CreateTable("SceneInfo");
		
		//tbNodeInfoC.AddField<QRD::NUMBER>("x");
		//tbNodeInfoC.AddField<QRD::NUMBER>("y");
		//tbNodeInfoC.AddField<QRD::NUMBER>("width");
		//tbNodeInfoC.AddField<QRD::NUMBER>("height");
		
		//to store Application::m_Zoom
		//tbSceneInfoC.AddField<QRD::NUMBER>("zoom");

		Button::SetOnButtonClickedCallback([this](Button& btn) { OnButtonClicked(btn); });
		Node::SetOnNodeClickedCallback([this](Node& node) { OnNodeClicked(node); });
		TextBox::SetOnTextBoxClickedCallback([this](TextBox& txtBox) { OnTextBoxClicked(txtBox); });
	}

	Application::~Application()
	{
		//QRD::Table& tbNodeInfo = m_Database.GetTable("NodeInfo");
		//QRD::Table& tbSceneInfo = m_Database.GetTable("SceneInfo");

		for (auto* control : m_Window.GetControls())
		{
			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			//tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height);
		}
		
		//tbSceneInfo.AddRecord(m_Zoom);

		//m_Database.ExitDb();
	}

	int Application::Run()
	{
		return m_Window.ProcessMessage();
	}

	void Application::OnButtonClicked(Button& control)
	{
		//TODO: Implement functions bellow this point
		//WARNING: OnButtonClicked is only called once, thus Mouse::IsLeftPressed() is only checked once and the line only draws when clicking the button
		DrawLine(control);
	}

	void Application::OnNodeClicked(Node& node)
	{
		//TODO: Implement functions bellow this point
		//WARNING: Same as in Application::OnButtonClicked (cannot detect "ControlHoldMouseEvent")
		MoveNodesWithMouse(node);
		ResizeNodes(node);
	}

	void Application::OnTextBoxClicked(TextBox& txtBox)
	{
		if (Mouse::IsLeftPressed())
		{

		}
	}

	void Application::OnEvent(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsLeftPressed())
		{
			m_MousePos = Mouse::GetPos();
		}

		MoveNodes(e);
		Zoom(e);
		OnPaint(e);
		NewNode(e);
	}

	void Application::NewNode(const Event& e)
	{
		if (e.GetType() == EventType::KeyPressedEvent)
		{
			//Shift + A
			if (Keyboard::IsKeyPressed(VK_SHIFT) && Keyboard::IsKeyPressed(65))
			{
				Control* control = m_Window.AddControl(new Node(m_Window.Renderer2D, Mouse::GetPos(), { 200.0f * m_Zoom, 150.0f * m_Zoom }, { 15.0f, 16.0f, 19.0f }));
				
				m_Window.Renderer2D.BeginDraw();
				control->Render();
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::MoveNodesWithMouse(Node& node)
	{
		NPoint diff{};
		diff.x = Mouse::GetPos().x - m_MousePos.x;
		diff.y = Mouse::GetPos().y - m_MousePos.y;

		node.MoveBy(diff);

		m_MousePos = Mouse::GetPos();

		m_Window.Renderer2D.BeginDraw();
		m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
		
		for (auto* control : m_Window.GetControls())
			control->Render();
		
		m_Window.Renderer2D.EndDraw();
	}

	void Application::ResizeNodes(Node& node)
	{
		////TODO: Implement function (not required)
		//
		////If mouse is in the lower right corner of the node
		//const NPoint& mousePos = Mouse::GetPos();
		//const NPoint& nodePos = node.GetPos();
		//const NSize& nodeSize = node.GetSize();
		//
		//if (mousePos.x < nodePos.x + nodeSize.width && mousePos.x > nodePos.x + nodeSize.width - 100)
		//{
		//	SetCursor(LoadCursor(NULL, IDC_CROSS));
		//	NPoint diff{};
		//	diff.x = Mouse::GetPos().x - m_MousePos.x;
		//	diff.y = Mouse::GetPos().y - m_MousePos.y;
		//
		//	
		//	node.ResizeTo({ nodeSize.width + diff.x, nodeSize.height + diff.y });
		//
		//	m_Window.Renderer2D.BeginDraw();
		//	for (auto* control : m_Window.GetControls())
		//	{
		//		control->Render();
		//	}
		//	m_Window.Renderer2D.EndDraw();
		//}
		//SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	void Application::DrawLine(const Button& btn)
	{
		if(Mouse::IsLeftPressed())
		{
			m_Window.Renderer2D.BeginDraw();
		
			m_Window.Renderer2D.RenderScene({ 35.0f, 37.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
				control->Render();

			NPoint btnPos = { btn.GetPos().x + btn.GetSize().width / 2, btn.GetPos().y + btn.GetSize().height / 2 };
			m_Window.Renderer2D.RenderLine(btnPos, Mouse::GetPos(), { 255.0f, 255.0f, 255.0f }, 2);
			
			m_Window.Renderer2D.EndDraw();
		}

	}

	void Application::OnPaint(const Event& e)
	{
		if (e.GetType() == EventType::AppPaintEvent)
		{
			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene(NColor{ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
				control->Render();
			m_Window.Renderer2D.EndDraw();
		}
	}

	void Application::MoveNodes(const Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent && Mouse::IsMiddlePressed())
		{
			m_MousePos = Mouse::GetPos();
		}

		else if (e.GetType() == EventType::MouseMoveEvent && Mouse::IsMiddlePressed())
		{
			NPoint diff{};
			diff.x = Mouse::GetPos().x - m_MousePos.x;
			diff.y = Mouse::GetPos().y - m_MousePos.y;

			int mBuff = 0;
			if (diff.x > mBuff || diff.y > mBuff || diff.x < -mBuff || diff.y < -mBuff)
			{
				m_MousePos.x = Mouse::GetPos().x;
				m_MousePos.y = Mouse::GetPos().y;

				m_Window.Renderer2D.BeginDraw();
				m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
				for (auto* control : m_Window.GetControls())
				{
					control->MoveBy(diff);
					control->Render();
				}
				m_Window.Renderer2D.EndDraw();
			}
		}
	}

	void Application::Zoom(const Event& e)
	{
		NPoint screenCenter = Mouse::GetPos();

		if (e.GetType() == EventType::MouseWheelUpEvent)
		{
			m_Zoom += 0.02f;

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for (auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= -m_ZoomFactor;
				newPos.y *= -m_ZoomFactor;

				newSize.width = size.width * m_ResizeFactor;
				newSize.height = size.height * m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
		else if (e.GetType() == EventType::MouseWheelDownEvent)
		{
			m_Zoom -= 0.02f;
			if (m_Zoom <= m_ZoomBoundary)
			{
				m_Zoom = m_ZoomBoundary;
				return;
			}

			m_Window.Renderer2D.BeginDraw();
			m_Window.Renderer2D.RenderScene({ 35.0f, 38.0f, 40.0f });
			for(auto* control : m_Window.GetControls())
			{
				const NPoint& pos = control->GetPos();
				const NSize& size = control->GetSize();
				NPoint newPos = screenCenter - pos;
				NSize newSize;
				newPos.x *= m_ZoomFactor;
				newPos.y *= m_ZoomFactor;

				newSize.width = size.width / m_ResizeFactor;
				newSize.height = size.height / m_ResizeFactor;
				control->MoveBy(newPos);
				control->ResizeTo(newSize);
				control->Render();
			}
			m_Window.Renderer2D.EndDraw();
		}
	}
}


