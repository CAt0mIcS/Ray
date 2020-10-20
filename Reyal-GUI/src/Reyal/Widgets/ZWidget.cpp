#include "rlpch.h"
#include "ZWidget.h"



namespace Zeal::Reyal
{
    bool Widget::operator==(const Widget& other)
    {
        return GetName() == other.GetName();
    }

    Widget* Widget::FindChild(const std::wstring_view name)
    {
        for (auto* child : m_Children)
        {
            if (child->GetName() == name)
                return child;
        }
        return nullptr;
    }

    Widget::~Widget()
    {
        //TODO: Delete children
    }

    Widget::Widget(const std::wstring_view name, _In_opt_ Widget* parent)
        : m_Parent(parent), m_Name(name)
    {
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

