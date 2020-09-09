#include "pch.h"
#include "Node.h"

#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"


using namespace Util;


namespace GUI
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(Control* parent)
		: Control(parent)
	{
	}

	bool Node::Render()
	{
		if (this->IsInWindow())
		{
			for (auto* child : m_Children)
			{
				child->Render();
			}

			const float max = std::max(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);

			return true;
		}
		return false;
	}

	bool Node::OnEvent(Event& e)
	{
		return false;
	}
	
}


