#include "pch.h"
#include "TextCursor.h"

#include "../TextBox.h"

#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/MouseEvent.h"

#include "GUI/Graphics/Renderer.h"


namespace GUI
{
    TextCursor::TextCursor(TextBox* parent)
        : Control((Control*)parent), m_TxtParent(parent), m_PosInText((unsigned int)parent->GetText().size() - 1), m_IsFocused(false)
    {
    }

    bool TextCursor::Render()
    {
        if (this->IsInWindow() && m_IsFocused)
        {
            float x = Mouse::GetPos().x;
            
            float y1 = m_TxtParent->GetPos().y + m_TxtParent->GetFontSize();

            Renderer::Get().RenderLine({ x, y1 - m_TxtParent->GetFontSize() / 2 }, { x, y1 + m_TxtParent->GetFontSize() / 2 }, { 255, 255, 255 }, 4);
            //TODO: Render cursor here
        }
        return false;
    }

    bool TextCursor::OnEvent(Event& e)
    {
        switch (e.GetType())
        {
        case EventType::KeyPressedEvent:
        {
        }
        case EventType::MouseButtonPressedEvent:
        {
            MouseButtonPressedEvent& evnt = (MouseButtonPressedEvent&)e;
            if (evnt.GetButton() == MouseButton::Left)
            {
                m_IsFocused = true;
                Renderer::Get().BeginDraw();
                Render();
                Renderer::Get().EndDraw();
                return true;
            }
            break;
        }
        }
        return false;
    }

    //TODO: Implement function
    std::optional<std::pair<Util::NPoint, Util::NSize>> TextCursor::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
    {
        return { {{}, {}} };
    }
}
