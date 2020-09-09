#include "pch.h"
#include "TextCursor.h"

#include "../TextBox.h"

#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/MouseEvent.h"


namespace GUI
{
    TextCursor::TextCursor(TextBox* parent)
        : Control((Control*)parent), m_TxtParent(parent), m_PosInText((unsigned int)parent->GetText().size() - 1)
    {
    }

    bool TextCursor::Render()
    {
        if (this->IsInWindow())
        {
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
