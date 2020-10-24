#include "rlpch.h"
#include "ZWidget.h"

#include "Reyal/Input/Mouse.h"


namespace Zeal::Reyal
{
    bool Widget::operator==(const Widget& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    void Widget::MoveBy(const Util::Point& pos)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::MoveTo(const Util::Point& pos)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::ResizeBy(const Util::Size& size)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::ResizeTo(const Util::Size& size)
    {
        ZL_PROFILE_FUNCTION();

    }

    std::shared_ptr<Widget> Widget::FindChild(const std::wstring_view name)
    {
        ZL_PROFILE_FUNCTION();
        for (auto child : m_Children)
        {
            if (child->GetName() == name)
                return child;
        }
        return nullptr;
    }

    Widget::~Widget()
    {
        ZL_PROFILE_FUNCTION();
        //TODO: Delete children
    }

    Widget::Widget(const std::wstring_view name, _In_opt_ std::shared_ptr<Widget> parent)
        : m_Name(name), m_Parent(parent), m_Renderer(nullptr)
    {
        ZL_PROFILE_FUNCTION();
    }

    Widget* Widget::GetEventReceiver(const Event& e, const Mouse& mouse)
    {
        ZL_PROFILE_FUNCTION();

        Widget* receiver = nullptr;
        if (mouse.IsOnWidget(this))
        {
            for (auto child : m_Children)
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

