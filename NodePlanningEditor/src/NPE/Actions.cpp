#include "Actions.h"

#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Debug/Logger.h"


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

		for (auto* control : app.m_Window.GetControls())
		{
			control->MoveBy(diff);
		}
		app.m_Window.PostRedraw();
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
				width *= s_ResizeFactor;
				height *= s_ResizeFactor;
			}
		}
		else
		{
			for (int i = app.m_Zoom; i < 0; ++i)
			{
				width /= s_ResizeFactor;
				height /= s_ResizeFactor;
			}
		}

		GUI::Node* control = app.m_Window.AddControl<GUI::Node>(new GUI::Node(&app.m_Window));
		control->SetColor(GUI::g_DefaultNodeColor);
		control->SetSize({ width, height });
		control->SetPos(GUI::Mouse::GetPos());
		control->Init();

		NPE_LOG("Created Node: \nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}", control->GetPos().x, control->GetPos().y, control->GetSize().width, control->GetSize().height);

		app.m_Window.PostRedraw();
	}

	void Actions::MoveNodes(GUI::Node* node)
	{
		auto& app = *m_App;

		Util::NPoint diff{};
		diff.x = GUI::Mouse::GetPos().x - app.m_MousePos.x;
		diff.y = GUI::Mouse::GetPos().y - app.m_MousePos.y;

		node->MoveBy(diff);

		app.m_MousePos = GUI::Mouse::GetPos();

		app.m_Window.PostRedraw();
	}

	void Actions::ZoomIn()
	{
		Util::NPoint center = GUI::Mouse::GetPos();
		++m_App->m_Zoom;

		if (m_App->m_Zoom >= s_ZoomBoundary)
		{
			m_App->m_Zoom = s_ZoomBoundary;
			return;
		}

		for (auto* control : m_App->m_Window.GetControls())
		{
			control->MoveBy((center - control->GetPos()) * -s_ZoomFactor);
			control->ResizeTo(control->GetSize() * s_ResizeFactor);
		}

		m_App->m_NeedsToSave = true;
		m_App->m_Window.PostRedraw();
	}

	void Actions::ZoomOut()
	{
		Util::NPoint center = GUI::Mouse::GetPos();

		--m_App->m_Zoom;
		if (m_App->m_Zoom <= -s_ZoomBoundary)
		{
			m_App->m_Zoom = -s_ZoomBoundary;
			return;
		}

		for (auto* control : m_App->m_Window.GetControls())
		{
			control->MoveBy((center - control->GetPos()) * s_ZoomFactor);
			control->ResizeTo(control->GetSize() / s_ResizeFactor);
		}

		m_App->m_NeedsToSave = true;
		m_App->m_Window.PostRedraw();
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

			float fontsize = line.first->GetSize().width / 3;
			if (fontsize == 0) fontsize = 1;

			GUI::Renderer::Get().RenderLine({ x1, y1 }, { x2, y2 }, GUI::g_DefaultLineColor, fontsize);
		}
	}
	
	void Actions::FinnishLineDrawing()
	{
		if (m_App->m_Lines.size() == 0)
			return;
			
		for (auto* control : m_App->m_Window.GetControls())
		{
			if (control->IsInWindow() && control->GetType() == GUI::Control::Type::Node)
			{
				GUI::Button* btn = (GUI::Button*)control->GetChildren()[2];
				if (GUI::Mouse::IsOnControl(btn))
				{
					m_App->m_Lines[m_App->m_Lines.size() - 1].second = btn;
					break;
				}
			}
		}
		if (m_App->m_Lines.size() > 0 && m_App->m_Lines[m_App->m_Lines.size() - 1].second == nullptr)
		{
			m_App->m_Lines.erase(m_App->m_Lines.end() - 1);
			m_App->m_Window.PostRedraw();
		}
		else
		{
			m_App->m_NeedsToSave = true;
			auto line = m_App->m_Lines[m_App->m_Lines.size() - 1];
			NPE_LOG("Connected Line: \nStart:\t{0}\nEnd:\t{1}", line.first->GetPos(), line.second->GetPos());
		}
		m_App->m_DrawLines = false;
	}
	
	void Actions::EraseLine()
	{
		auto linesIntersect = [](const Util::NPoint& p1, const Util::NPoint& p2, const Util::NPoint& q1, const Util::NPoint& q2)
		{
			return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
				* ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
				&&
				(((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
					* ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0);
		};

		for (unsigned int i = 0; i < m_App->m_Lines.size(); ++i)
		{
			if (linesIntersect(m_App->m_Lines[i].first->GetPos(), m_App->m_Lines[i].second->GetPos(), m_App->m_MousePos, GUI::Mouse::GetPos()))
			{
				m_App->m_Lines.erase(m_App->m_Lines.begin() + i);
				m_App->m_NeedsToSave = true;
			}
		}

		m_App->m_Window.PostRedraw();
	}
	
	void Actions::DeleteNode(GUI::Node* watched)
	{
		auto& controls = m_App->m_Window.GetControls();
		auto& lines = m_App->m_Lines;

		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			if (controls[i]->GetId() == watched->GetId())
			{
				GUI::Button* lineBtn = (GUI::Button*)controls[i]->GetChildren()[2];

				for (unsigned int j = 0; j < lines.size(); ++j)
				{
					if (lines[j].first == lineBtn || lines[j].second == lineBtn)
					{
						m_App->m_Lines.erase(lines.begin() + j);
					}
				}

				delete controls[i];
				controls.erase(controls.begin() + i);
				m_App->m_NeedsToSave = true;

				m_App->m_Window.PostRedraw();

				break;
			}
		}
	}
	
	void Actions::ScrollUp(GUI::TextBox* watched)
	{

	}
	
	void Actions::ScrollDown(GUI::TextBox* watched)
	{

	}
}


