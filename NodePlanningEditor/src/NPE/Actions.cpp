#include "Actions.h"

#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"


namespace NPE
{
	Actions::Actions(Application* app)
		: m_App(app)
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
		GUI::Renderer::Get().EndDraw();
	}
	
	void Actions::ZoomIn()
	{
		auto& app = *m_App;
		Util::NPoint center = GUI::Mouse::GetPos();

		++app.m_Zoom;
		GUI::Renderer::Get().BeginDraw();
		for (auto* control : app.m_Window.GetControls())
		{
			const Util::NPoint& pos = control->GetPos();
			const Util::NSize& size = control->GetSize();
			Util::NPoint newPos = center - pos;
			Util::NSize newSize;
			newPos.x *= -app.s_ZoomFactor;
			newPos.y *= -app.s_ZoomFactor;

			newSize.width = size.width * app.s_ResizeFactor;
			newSize.height = size.height * app.s_ResizeFactor;
			control->MoveBy(newPos);
			control->ResizeTo(newSize);
			control->Render();
		}
		app.m_Window.Render();
		GUI::Renderer::Get().EndDraw();
	}

	void Actions::ZoomOut()
	{
		auto& app = *m_App;
		Util::NPoint center = GUI::Mouse::GetPos();

		--app.m_Zoom;
		if (app.m_Zoom <= app.s_ZoomBoundary)
		{
			app.m_Zoom = app.s_ZoomBoundary;
			return;
		}

		GUI::Renderer::Get().BeginDraw();
		for (auto* control : app.m_Window.GetControls())
		{
			const Util::NPoint& pos = control->GetPos();
			const Util::NSize& size = control->GetSize();
			Util::NPoint newPos = center - pos;
			Util::NSize newSize;
			newPos.x *= app.s_ZoomFactor;
			newPos.y *= app.s_ZoomFactor;

			newSize.width = size.width / app.s_ResizeFactor;
			newSize.height = size.height / app.s_ResizeFactor;
			control->MoveBy(newPos);
			control->ResizeTo(newSize);
		}
		app.m_Window.Render();
		GUI::Renderer::Get().EndDraw();
	}
}


