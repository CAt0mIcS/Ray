#include "Actions.h"

#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"


namespace NPE
{
	Actions::Actions(Application* app)
		: m_App(app), m_Zoom(0)
	{

	}

	void Actions::MoveCamera()
	{
		auto& app = *m_App;

		Util::NPoint diff = GUI::Mouse::GetPos() - app.m_MousePos;
		app.m_MousePos = GUI::Mouse::GetPos();

		GUI::Renderer::Get().BeginDraw();
		GUI::Renderer::Get().RenderScene();
		for (auto* control : app.m_Window.GetControls())
		{
			control->MoveBy(diff);
			control->Render();
		}

		RenderLines();
		GUI::Renderer::Get().EndDraw();
	}

	void Actions::SpawnNode()
	{
		auto& app = *m_App;

		float width = app.s_NodeWidth;
		float height = app.s_NodeHeight;

		if (app.m_Zoom > 0)
		{
			for (int i = 0; i < app.m_Zoom; ++i)
			{
				width *= app.s_ResizeFactor;
				height *= app.s_ResizeFactor;
			}
		}
		else
		{
			for (int i = app.m_Zoom; i < 0; ++i)
			{
				width /= app.s_ResizeFactor;
				height /= app.s_ResizeFactor;
			}
		}

		GUI::Node* control = app.m_Window.AddControl<GUI::Node>(new GUI::Node(&app.m_Window));
		control->SetColor(GUI::g_DefaultNodeColor);
		control->SetSize({ width, height });
		control->SetPos(GUI::Mouse::GetPos());
		control->Init();

		GUI::Renderer::Get().BeginDraw();
		control->Render();
		GUI::Renderer::Get().EndDraw();
	}

	void Actions::MoveNodes(GUI::Node* node)
	{
		auto& app = *m_App;

		Util::NPoint diff{};
		diff.x = GUI::Mouse::GetPos().x - app.m_MousePos.x;
		diff.y = GUI::Mouse::GetPos().y - app.m_MousePos.y;

		node->MoveBy(diff);

		app.m_MousePos = GUI::Mouse::GetPos();

		GUI::Renderer::Get().BeginDraw();
		app.m_Window.Render();
		RenderLines();
		GUI::Renderer::Get().EndDraw();
	}

	/*
	* TODO: Look at ID2D1RenderTarget::SetTransform
	*	https://docs.microsoft.com/en-us/windows/win32/direct2d/id2d1rendertarget-settransform?redirectedfrom=MSDN
	*	https://stackoverflow.com/questions/23228990/implement-a-simple-lookat-like-camera-in-direct2d
	*
	*	https://gamedev.stackexchange.com/questions/46228/implementing-a-camera-viewport-to-a-2d-game
	*/
	void Actions::ZoomIn()
	{
		Util::NPoint center = GUI::Mouse::GetPos();
		++m_Zoom;

		if (m_Zoom >= s_ZoomBoundary)
		{
			m_Zoom = s_ZoomBoundary;
			return;
		}

		GUI::Renderer::Get().BeginDraw();
		for (auto* control : m_App->m_Window.GetControls())
		{
			std::cout << "Control before: Center: " << center << ' ' << control->GetPos() << ' ' << control->GetSize() << '\n';
			control->MoveBy((center - control->GetPos()) * -s_ZoomFactor);
			control->ResizeTo(control->GetSize() * s_ResizeFactor);
			std::cout << "Control after: Center: " << center << ' ' << control->GetPos() << ' ' << control->GetSize() << '\n';
		}

		m_App->m_Window.Render();
		RenderLines();
		GUI::Renderer::Get().EndDraw();
	}

	void Actions::ZoomOut()
	{
		Util::NPoint center = GUI::Mouse::GetPos();

		--m_Zoom;
		if (m_Zoom <= -s_ZoomBoundary)
		{
			m_Zoom = -s_ZoomBoundary;
			return;
		}

		GUI::Renderer::Get().BeginDraw();
		for (auto* control : m_App->m_Window.GetControls())
		{
			control->MoveBy((center - control->GetPos()) * s_ZoomFactor);
			control->ResizeTo(control->GetSize() / s_ResizeFactor);
			std::cout << "Control before: Center: " << center << ' ' << control->GetPos() << ' ' << control->GetSize() << '\n';
		}

		m_App->m_Window.Render();
		RenderLines();
		GUI::Renderer::Get().EndDraw();

	}
	
	void Actions::DrawLine()
	{
		GUI::Button& startBtn = *m_App->m_Lines[m_App->m_Lines.size() - 1].first;
		Util::NPoint btnPos = { startBtn.GetPos().x + startBtn.GetSize().width / 2, startBtn.GetPos().y + startBtn.GetSize().height / 2 };
		
		GUI::Renderer::Get().BeginDraw();

		m_App->m_Window.Render();

		GUI::Renderer::Get().RenderLine(btnPos, GUI::Mouse::GetPos(), GUI::g_DefaultLineColor, (unsigned int)startBtn.GetSize().width / 3);
		GUI::Renderer::Get().EndDraw();
	}
	
	void Actions::RenderLines()
	{
		for (Line& line : m_App->m_Lines)
		{
			float x2 = GUI::Mouse::GetPos().x;
			float y2 = GUI::Mouse::GetPos().y;
			if (line.second)
			{
				x2 = line.second->GetPos().x + line.second->GetSize().width / 2;
				y2 = line.second->GetPos().y + line.second->GetSize().height / 2;
			}
			
			float x1 = line.first->GetPos().x + line.first->GetSize().width / 2;
			float y1 = line.first->GetPos().y + line.first->GetSize().height / 2;

			unsigned int fontsize = (unsigned int)(line.first->GetSize().width / 3);
			if (fontsize == 0) fontsize = 1;

			GUI::Renderer::Get().RenderLine({ x1, y1 }, { x2, y2 }, GUI::g_DefaultLineColor, fontsize);
		}
	}
}


