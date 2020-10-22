#include "rlpch.h"
#include "ZWidget.h"

#include "Reyal/Input/Mouse.h"


namespace Zeal::Reyal
{
    bool Widget::operator==(const Widget& other)
    {
        return GetName() == other.GetName();
    }

    void Widget::MoveBy(const Util::Point& pos)
    {

    }

    void Widget::MoveTo(const Util::Point& pos)
    {

    }

    void Widget::ResizeBy(const Util::Size& size)
    {

    }

    void Widget::ResizeTo(const Util::Size& size)
    {

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
        : m_Name(name), m_Parent(parent)
    {
    }

    Widget* Widget::GetEventReceiver(const Event& e, const Mouse& mouse)
    {
        Widget* receiver = nullptr;
        if (mouse.IsOnWidget(this))
        {
            for (auto* child : m_Children)
            {
                receiver = child->GetEventReceiver(e, mouse);
                if (receiver)
                    break;
            }

            if (!receiver)
                receiver = this;
        }

        return receiver;
    }
}

