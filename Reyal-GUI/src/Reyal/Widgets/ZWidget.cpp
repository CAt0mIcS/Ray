#include "rlpch.h"
#include "ZWidget.h"


namespace Zeal::Reyal
{
    uint32_t ZWidget::s_NextID = 0;

    ZWidget::ZWidget(_In_opt_ ZWidget* parent)
        : m_Parent(parent), m_ID(s_NextID)
    {
        ++s_NextID;
    }
}
