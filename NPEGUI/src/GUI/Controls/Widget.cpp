#include "pch.h"
#include "Widget.h"

#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	Widget::Widget(_In_opt_ Control* parent)
		: Control(Type::Widget, parent)
	{
	}
	
	bool Widget::Render()
	{
		if (this->IsInWindow())
		{
			if (m_RenderCallback)
			{
				m_RenderCallback(this);
				return true;
			}

			Renderer::Get().RenderRect(m_Pos, m_Size, Constants::g_DefaultWidgetColor);
			return true;
		}
		return false;
	}
	
	bool Widget::OnEvent(Event& e)
	{
		return false;
	}
}


