#include "Ray/Rpch.h"
#include "RButton.h"

namespace At0::Ray
{
	Button::Button(const std::string_view name, Widget& parent)
		: Widget(name, &parent)
	{

	}

	void Button::AddPaintEventListener(EventDispatcher<PaintEvent>& dispatcher)
	{
		EventListener<PaintEvent>::Subscribe(dispatcher);
	}

	void Button::SetPos(const Point2& pos)
	{
	}

	void Button::SetSize(const Size2& size)
	{
	}

	Button::~Button()
	{
	}

	void Button::OnEvent(Widget* receiver, PaintEvent& e)
	{
	}
}
