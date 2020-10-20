#include "rlpch.h"
#include "ZWidget.h"

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"


namespace Zeal::Reyal
{
    uint32_t Widget::s_NextID = 0;

    Widget::Widget(_In_opt_ Widget* parent)
        : m_Parent(parent), m_ID(s_NextID)
    {
        ++s_NextID;
    }
    
    Widget* Widget::GetEventReceiver(const Event& e)
    {
        Widget* receiver = nullptr;
        for (auto* child : m_Children)
        {
            receiver = child->GetEventReceiver(e);
            if (receiver)
                break;
        }

        if (!receiver)
            receiver = this;

        return receiver;
    }
}
