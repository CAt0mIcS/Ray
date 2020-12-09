#include "rpch.h"
#include "Ray/Widgets/Widget.h"

#include "Ray/Input/Mouse.h"

#include <RayDebug/RInstrumentor.h>
#include <RayUtil/Exception.h>
#include <RayUtil/TypeConvert.h>


namespace At0::Ray
{
    void Widget::MoveBy(const Point2& pos)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::MoveTo(const Point2& pos)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::ResizeBy(const Size2& size)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::ResizeTo(const Size2& size)
    {
        RL_PROFILE_FUNCTION();

    }

    Widget* Widget::FindChild(const std::string_view name)
    {
        RL_PROFILE_FUNCTION();
        for (auto& child : m_Children)
        {
            if (child->GetName() == name)
                return child.get();
        }
        return nullptr;
    }

    void Widget::PostRedraw(bool eraseBkgnd) const
    {
        void* hWnd = GetNativeWindow();
        
        //TODO: Program should be able to continue if one call fails

        //TODO: Invalidate only widget area
        //TODO: Make Platform independent
        // RL_THROW_LAST_WND_EXCEPT(InvalidateRect(hWnd, &Util::TransformationToRect(m_Matrix), (BOOL)eraseBkgnd));
        // RL_THROW_LAST_WND_EXCEPT(UpdateWindow(hWnd));
    }

    /// <summary>
    /// Defines commonly used operators
    /// </summary>
    #pragma region Operators

    bool Widget::operator==(const Widget& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator==(const Scope<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator==(const Ref<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Widget& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator!=(const Scope<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Ref<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }
    
    #pragma endregion

    Widget::~Widget()
    {
        RL_PROFILE_FUNCTION();
    }

    Widget::Widget(const std::string_view name, Widget* parent)
        : m_Name(name), m_Parent(parent), m_Children{}, m_Matrix{}
    {
        RL_PROFILE_FUNCTION();
    }

    Widget* Widget::GetEventReceiver(const Event& e, const MouseInput& mouse)
    {
        RL_PROFILE_FUNCTION();

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

