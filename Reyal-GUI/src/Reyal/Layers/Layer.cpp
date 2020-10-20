#include "rlpch.h"
#include "Layer.h"



namespace Zeal::Reyal
{
    Layer::Layer(const std::wstring_view name, _In_opt_ Widget* parent)
        : Widget(name, parent), m_Name(name)
    {
    }
}

