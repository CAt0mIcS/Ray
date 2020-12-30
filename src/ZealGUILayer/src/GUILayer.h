#pragma once

#include "glbase.h"

#include <Ray/Ray.h>


namespace At0::Layers
{
	class ZGL_API GUILayer : public Ray::Layer,
		Ray::EventListener<Ray::MouseMoveEvent>,
		Ray::EventListener<Ray::WindowCloseEvent>,
		Ray::EventListener<Ray::KeyPressedEvent>,
		Ray::EventListener<Ray::CharEvent>,

		Ray::EventListener<Ray::MouseButtonPressedEvent>,

		Ray::EventListener<Ray::MouseWheelUpEvent>,
		Ray::EventListener<Ray::MouseWheelDownEvent>,
		Ray::EventListener<Ray::MouseWheelLeftEvent>,
		Ray::EventListener<Ray::MouseWheelRightEvent>
	{
	public:
		GUILayer(std::string_view name);

		virtual void OnUpdate(Ray::Timestep ts) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseMoveEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::WindowCloseEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::KeyPressedEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::CharEvent& e) override;

		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseButtonPressedEvent& e) override;

		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseWheelUpEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseWheelDownEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseWheelLeftEvent& e) override;
		virtual void OnEvent(Ray::Widget* receiver, Ray::MouseWheelRightEvent& e) override;
	};
}

extern "C"
{
	ZGL_API At0::Layers::GUILayer* CreateLayer()
	{
		return new At0::Layers::GUILayer("GUI-Layer");
	}
}

