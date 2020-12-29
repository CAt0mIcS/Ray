#include "Ray/Rpch.h"
#include "RButton.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	// -------------------------------------------------------------------------
	// Button
	Button::Button(const std::string_view name, Widget& parent)
		: Widget(name, &parent), m_Pos{}, m_Size{}
	{

	}

	void Button::AddPaintEventDispatcher(EventDispatcher<PaintEvent>& dispatcher)
	{
		RAY_MEXPECTS(!EventListener<PaintEvent>::HasDispatcher(dispatcher), "[Button::AddPaintEventDispatcher] Dispatcher already added.");
		EventListener<PaintEvent>::Subscribe(dispatcher);
	}

	Renderer3D& Button::GetRenderer3D() const
	{
		RAY_MEXPECTS(m_Parent, "[Button::GetRenderer3D] No parent was found at 0x{0}", m_Parent);
		return m_Parent->GetRenderer3D();
	}

	Button::~Button()
	{
	}

	// -------------------------------------------------------------------------
	// PushButton
	PushButton::PushButton(const std::string_view name, Widget& parent)
		: Button(name, parent)
	{
	}

	void PushButton::OnEvent(Widget* receiver, PaintEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[PushButton] [{0}] '{1}' received PaintEvent", GetName(), receiver->GetName());
	}
}
