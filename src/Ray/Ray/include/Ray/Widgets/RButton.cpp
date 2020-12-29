#include "Ray/Rpch.h"
#include "RButton.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>

#include <RayRender/Drawable/Rectangle.h>


namespace At0::Ray
{
	// -------------------------------------------------------------------------
	// Button
	Button::Button(const std::string_view name, Widget& parent)
		: Widget(name, &parent), m_Pos{}, m_Size{}
	{

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
		float col[] = { 255, 0, 0 };
		m_DrawObject = new Rectangle(parent.GetRenderer3D(), 2.0f, col);
	}

	void PushButton::Draw()
	{
		Point2 pos = GetPos();
		Size2 size = GetSize();
		m_DrawObject->SetTranslation(pos.x, pos.y, 0.0f);
		m_DrawObject->SetScale(size.x, size.y, 0.0f);
		m_DrawObject->Draw(&m_Parent->GetRenderer3D());
	}
}
