#include "Ray/Rpch.h"
#include "RButton.h"

#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>

#include <RayUtil/RHelper.h>


namespace At0::Ray
{
	// -------------------------------------------------------------------------
	// Button
	Button::Button(const std::string_view name, Widget& parent)
		: Widget(name, &parent)
	{

	}

	Renderer3D& Button::GetRenderer3D() const
	{
		RAY_MEXPECTS(m_Parent, "[Button::GetRenderer3D] No parent was found at 0x{0}", m_Parent);
		return m_Parent->GetRenderer3D();
	}

	Button::~Button()
	{
		Log::Info("[Button] '{0}' destroyed", GetName());
	}

	// -------------------------------------------------------------------------
	// PushButton
	PushButton::PushButton(const std::string_view name, Widget& parent)
		: Button(name, parent)
	{
		float col[] = { 255, 0, 0 };
		//m_DrawObject = new Rectangle(GetRenderer3D(), 1.0f, col);
	}

	PushButton::~PushButton()
	{
		//delete m_DrawObject;
		Log::Info("[PushButton] '{0}' destroyed", GetName());
	}

	void PushButton::Draw()
	{
		//m_DrawObject->Draw(&GetRenderer3D());
	}

	void PushButton::Move(const Point2& pos)
	{
		RECT rc;
		GetClientRect((HWND)GetNativeWindow(), &rc);
		Util::CoordinateTransformer<Point2> transformer({ float(rc.right - rc.left), float(rc.bottom - rc.top) });

		const Size2& size = GetSize();
		Point2 newPos = pos;
		newPos.x += size.x / 2.0f;
		newPos.y += size.y / 2.0f;
		newPos = transformer.ToNormalizedDeviceCoordinate(newPos);

		//TransformComponent& tform = m_DrawObject->GetComponent<TransformComponent>();
		//tform.Translation = { newPos.x, newPos.y, 0.0f };
	}

	void PushButton::Resize(const Size2& size)
	{
		// RAY_TODO: Platform independent!
		RECT rc;
		GetClientRect((HWND)GetNativeWindow(), &rc);

		Size2 newSize = size;
		newSize.x /= (rc.right - rc.left);
		newSize.y /= (rc.bottom - rc.top);

		//TransformComponent& tform = m_DrawObject->GetComponent<TransformComponent>();
		//tform.Scale = { newSize.x, newSize.y, 0.0f };
	}

	Point2 PushButton::GetPos() const
	{
		RECT rc;
		GetClientRect((HWND)GetNativeWindow(), &rc);
		Util::CoordinateTransformer<Point2> transformer({ float(rc.right - rc.left), float(rc.bottom - rc.top) });

		//TransformComponent& tform = m_DrawObject->GetComponent<TransformComponent>();
		//Point2 pos = transformer.ToPixelCoordinate({ tform.Translation.x, tform.Translation.y });
		//Size2 size = GetSize();
		//pos.x -= size.x / 2.0f;
		//pos.y -= size.y / 2.0f;

		//return pos;
		return {};
	}

	Size2 PushButton::GetSize() const
	{
		RECT rc;
		GetClientRect((HWND)GetNativeWindow(), &rc);

		//TransformComponent& tform = m_DrawObject->GetComponent<TransformComponent>();
		//Size2 size{ tform.Scale.x, tform.Scale.y };
		//size.x *= (rc.right - rc.left);
		//size.y *= (rc.bottom - rc.top);

		//return size;
		return {};
	}
}
