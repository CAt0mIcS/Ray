#pragma once

#include <Reyal/Layers/Layer.h>


namespace Zeal
{
	class GUILayer : public Reyal::Layer
	{
	public:
		GUILayer() = default;

		virtual void OnUpdate() override;
	};
}



