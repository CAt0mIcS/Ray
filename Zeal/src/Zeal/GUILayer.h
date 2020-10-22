#pragma once

#include <Reyal/Layers/Layer.h>

namespace Zeal::Reyal
{
	class Window;
}


namespace Zeal
{
	class GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::wstring_view name, Reyal::Window& win);

		virtual void OnUpdate() override;
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::Event& e) override;

	private:
		Reyal::Window& m_Win;
	};
}



