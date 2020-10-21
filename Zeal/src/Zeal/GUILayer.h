#pragma once

#include <Reyal/Layers/Layer.h>


namespace Zeal
{
	class GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::wstring_view name, Reyal::WindowRenderer& renderer);

		virtual void OnUpdate() override;
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::Event& e) override;
	};
}



