#pragma once

#include <Reyal/Layers/Layer.h>


namespace Zeal
{
	class GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::string_view name);

		virtual void OnUpdate() override;
	};
}



