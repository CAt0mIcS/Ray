#include "Ray/rpch.h"
#include "Ray/Widgets/RWidget.h"

#include "Ray/Input/RMouse.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/RException.h>
#include <RayUtil/RTypeConvert.h>


namespace At0::Ray
{
    Widget& Widget::FindChildByName(std::string_view name)
    {
        RAY_PROFILE_FUNCTION();
        for (auto& child : m_Children)
        {
            if (child->GetName() == name)
                return *child.get();
        }
        RAY_THROW_RUNTIME("[Widget::FindChild] was unable to find the child with name '{0}'", name);
    }

    void Widget::PostRedraw(bool eraseBkgnd) const
    {
        //RAY_TODO: Program should be able to continue if one call fails

        //RAY_TODO: Invalidate only widget area
        //RAY_TODO: Make Platform independent
        // RAY_WND_THROW_LAST_FAILED(InvalidateRect(hWnd, &Util::TransformationToRect(m_Matrix), (BOOL)eraseBkgnd));
        // RAY_WND_THROW_LAST_FAILED(UpdateWindow(hWnd));
    }

    bool Widget::operator==(const Widget& other) const
    {
        RAY_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator!=(const Widget& other) const
    {
        RAY_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    Widget::~Widget()
    {
        RAY_PROFILE_FUNCTION();
        RAY_LOG_DEBUG("[Widget] '{0}' destroyed", this->GetName());
    }

    Widget::Widget(std::string_view name, Widget* parent)
        : m_Name(name), m_Parent(parent), m_Children{}
    {
        RAY_PROFILE_FUNCTION();
    }

    Widget* Widget::GetEventReceiver(const Event& e, const MouseInput& mouse)
    {
        RAY_PROFILE_FUNCTION();

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

