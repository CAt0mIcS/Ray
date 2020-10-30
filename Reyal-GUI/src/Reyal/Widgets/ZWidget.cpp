#include "rlpch.h"
#include "ZWidget.h"

#include "Reyal/Input/Mouse.h"


namespace At0::Reyal
{
    void Widget::MoveBy(const Point& pos)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::MoveTo(const Point& pos)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::ResizeBy(const Size& size)
    {
        ZL_PROFILE_FUNCTION();

    }

    void Widget::ResizeTo(const Size& size)
    {
        ZL_PROFILE_FUNCTION();

    }

    Widget* Widget::FindChild(const std::wstring_view name)
    {
        ZL_PROFILE_FUNCTION();
        for (auto& child : m_Children)
        {
            if (child->GetName() == name)
                return child.get();
        }
        return nullptr;
    }

    /// <summary>
    /// Defines commonly used operators
    /// </summary>
    #pragma region Operators

    bool Widget::operator==(const Widget& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator==(const Scope<Widget>& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator==(const Ref<Widget>& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Widget& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator!=(const Scope<Widget>& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Ref<Widget>& other)
    {
        ZL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }
    
    #pragma endregion

    Widget::~Widget()
    {
        ZL_PROFILE_FUNCTION();
    }

    Widget::Widget(const std::wstring_view name, _In_opt_ Widget* parent)
        : m_Name(name), m_Parent(parent), m_Children{}
    {
        ZL_PROFILE_FUNCTION();
    }

    Widget* Widget::GetEventReceiver(const Event& e, const Mouse& mouse)
    {
        ZL_PROFILE_FUNCTION();

        Widget* receiver = nullptr;

        //The main window should always receive the event if no other receiver is found
        if (!GetParent())
            receiver = this;

        if (mouse.IsOnWidget(this))
        {
            for (auto& child : m_Children)
            {
                Widget* receiver2 = child->GetEventReceiver(e, mouse);
                if (receiver2)
                {
                    receiver = receiver2;
                    break;
                }
            }

            if (!receiver)
                receiver = this;
        }

        return receiver;
    }
}

