#pragma once

#include "glbase.h"

#include <Reyal/Reyal.h>


namespace Zeal::Layers
{
	class ZGL_API GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::wstring_view name);

		virtual void OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e) override;
	};
}

extern "C"
{
	ZGL_API Zeal::Layers::GUILayer* CreateLayer()
	{
		return new Zeal::Layers::GUILayer(L"GUI-Layer");
	}
}