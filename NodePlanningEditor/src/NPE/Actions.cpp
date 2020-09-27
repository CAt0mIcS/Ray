#include "Actions.h"

#include "Application.h"

#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Node.h"

#include "GUI/Handlers/Mouse.h"

#include "Util/Debug/Logger.h"

#include "Util/Util.h"


namespace NPE
{
	Actions::Actions(Application& app)
		: m_App(app)
	{

	}

	void Actions::MoveCamera(const Util::NPoint& oldMousePos, std::vector<GUI::Control*>& controls)
	{
		Util::NPoint diff = GUI::Mouse::GetPos() - oldMousePos;
		for (auto* control : controls)
		{
			control->MoveBy(diff);
		}
	}

	void Actions::SpawnNode(GUI::MainWindow& win, float width, float height, int zoom)
	{
		if (zoom > 0)
		{
			for (int i = 0; i < zoom; ++i)
			{
				width *= s_ResizeFactor;
				height *= s_ResizeFactor;
			}
		}
		else
		{
			for (int i = zoom; i < 0; ++i)
			{
				width /= s_ResizeFactor;
				height /= s_ResizeFactor;
			}
		}

		GUI::Node* control = win.AddControl<GUI::Node>(new GUI::Node(&win));
		control->SetColor(GUI::g_DefaultNodeColor);
		control->SetSize({ width, height });
		control->SetPos(GUI::Mouse::GetPos());
		control->Init();

		NPE_LOG("Created Node: \nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}", control->GetPos().x, control->GetPos().y, control->GetSize().width, control->GetSize().height);
	}

	void Actions::MoveNodes(GUI::Node* node, const Util::NPoint& oldMousePos)
	{
		Util::NPoint diff{};
		diff = GUI::Mouse::GetPos() - oldMousePos;
		node->MoveBy(diff);
	}

	void Actions::ZoomIn(int& zoom, std::vector<GUI::Control*>& controls)
	{
		Util::NPoint center = GUI::Mouse::GetPos();
		++zoom;

		if (zoom >= s_ZoomBoundary)
		{
			zoom = s_ZoomBoundary;
			return;
		}

		for (auto* control : controls)
		{
			control->MoveBy((center - control->GetPos()) * -s_ZoomFactor);
			control->ResizeTo(control->GetSize() * s_ResizeFactor);
		}

	}

	void Actions::ZoomOut(int& zoom, std::vector<GUI::Control*>& controls)
	{
		Util::NPoint center = GUI::Mouse::GetPos();

		--zoom;
		if (zoom <= -s_ZoomBoundary)
		{
			zoom = -s_ZoomBoundary;
			return;
		}

		for (auto* control : controls)
		{
			control->MoveBy((center - control->GetPos()) * s_ZoomFactor);
			control->ResizeTo(control->GetSize() / s_ResizeFactor);
		}
	}
	
	void Actions::RenderLines(const std::vector<Line>& lines)
	{
		for (const Line& line : lines)
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
	
	void Actions::FinnishLineDrawing(std::vector<Line>& lines, const std::vector<GUI::Control*> controls)
	{
		if (lines.size() == 0)
			return;
			
		for (auto* control : controls)
		{
			if (control->IsInWindow() && control->GetType() == GUI::Control::Type::Node)
			{
				GUI::Button* btn = (GUI::Button*)control->GetChildren()[2];
				if (GUI::Mouse::IsOnControl(btn))
				{
					lines[lines.size() - 1].second = btn;
					break;
				}
			}
		}
		if (lines.size() > 0 && lines[lines.size() - 1].second == nullptr)
		{
			lines.erase(lines.end() - 1);
			//m_App->m_Window.PostRedraw();
		}
		else
		{
			//m_App->m_NeedsToSave = true;
			auto line = lines[lines.size() - 1];
			NPE_LOG("Connected Line: \nStart:\t{0}\nEnd:\t{1}", line.first->GetPos(), line.second->GetPos());
		}
		//m_App->m_DrawLines = false;
	}
	
	void Actions::EraseLine(std::vector<Line>& lines, const Util::NPoint& oldMousePos)
	{
		auto linesIntersect = [](const Util::NPoint& p1, const Util::NPoint& p2, const Util::NPoint& q1, const Util::NPoint& q2)
		{
			return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
				* ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
				&&
				(((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
					* ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0);
		};

		for (unsigned int i = 0; i < lines.size(); ++i)
		{
			if (linesIntersect(lines[i].first->GetPos(), lines[i].second->GetPos(), oldMousePos, GUI::Mouse::GetPos()))
			{
				lines.erase(lines.begin() + i);
				//m_App->m_NeedsToSave = true;
			}
		}

		//m_App->m_Window.PostRedraw();
	}
	
	void Actions::DeleteNode(GUI::Node* watched, std::vector<GUI::Control*>& controls, std::vector<Line>& lines)
	{
		for (unsigned int i = 0; i < controls.size(); ++i)
		{
			if (controls[i]->GetId() == watched->GetId())
			{
				GUI::Button* lineBtn = (GUI::Button*)controls[i]->GetChildren()[2];

				for (unsigned int j = 0; j < lines.size(); ++j)
				{
					if (lines[j].first == lineBtn || lines[j].second == lineBtn)
					{
						lines.erase(lines.begin() + j);
					}
				}

				delete controls[i];
				controls.erase(controls.begin() + i);
				//m_App->m_NeedsToSave = true;

				//m_App->m_Window.PostRedraw();

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


