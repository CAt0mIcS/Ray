#pragma once

#include "glbase.h"

#include <Reyal/Reyal.h>


namespace At0::Layers
{
	class ZGL_API GUILayer : public Reyal::Layer,
		Reyal::EventListener<Reyal::MouseMoveEvent>
	{
	public:
		GUILayer(const std::string_view name);

		virtual void OnUpdate(Reyal::Timestep ts) override;
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e) override;
	};
}

extern "C"
{
	ZGL_API At0::Layers::GUILayer* CreateLayer()
	{
		return new At0::Layers::GUILayer("GUI-Layer");
	}
}

