#include "rlpch.h"
#include "LayerStack.h"

#include "Reyal/Debug/Instrumentor.h"


namespace Zeal::Reyal
{
	void LayerStack::PushLayer(_In_ Layer* layer)
	{
		ZL_PROFILE_FUNCTION();
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(_In_ Layer* layer)
	{
		ZL_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			delete layer;
			m_Layers.erase(it);
		}
	}
}


