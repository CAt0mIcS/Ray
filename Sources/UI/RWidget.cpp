#include "Rpch.h"
#include "RWidget.h"

#include "Scene/REntity.h"
#include "Geometry/RMesh.h"

#include "Devices/RWindow.h"


namespace At0::Ray
{
	Float2 Widget::GetTranslation() const { return { GetX(), GetY() }; }

	float Widget::GetX() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return (NDCSpaceToScreenSpace(tform.Translation()) - (GetScale() / 2.0f)).x;
	}

	float Widget::GetY() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return (NDCSpaceToScreenSpace(tform.Translation()) - (GetScale() / 2.0f)).y;
	}

	Float2 Widget::GetScale() const { return { GetWidth(), GetHeight() }; }

	float Widget::GetWidth() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return tform.Scale().x * 0.5 * Window::Get().GetFramebufferSize().x;
	}

	float Widget::GetHeight() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return tform.Scale().y * 0.5 * Window::Get().GetFramebufferSize().y;
	}

	void Widget::SetTranslation(Float2 coords)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		// tform.SetTranslation({ ScreenSpaceToNDCSpace(coords + (GetScale() / 2.0f)), 0.0f });
		SetX(coords.x);
		SetY(coords.y);
	}

	void Widget::SetX(float x)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		tform.SetTranslation(
			{ ScreenSpaceToNDCSpaceX(x + (GetScale().x / 2.0f)), tform.Translation().y, 0.0f });
	}

	void Widget::SetY(float y)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		tform.SetTranslation(
			{ tform.Translation().x, ScreenSpaceToNDCSpaceY(y + (GetScale().y / 2.0f)), 0.0f });
	}

	void Widget::SetScale(Float2 scale)
	{
		SetWidth(scale.x);
		SetHeight(scale.y);
	}

	void Widget::SetWidth(float width)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		Float3 newScale{ width / (0.5f * Window::Get().GetFramebufferSize().x), tform.Scale().y,
			1.0f };
		tform.SetScale(newScale);
	}

	void Widget::SetHeight(float height)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		Float3 newScale{ tform.Scale().x, height / (0.5f * Window::Get().GetFramebufferSize().y),
			1.0f };
		tform.SetScale(newScale);
	}

	Widget::Widget(Entity entity) : Component(entity) {}
}  // namespace At0::Ray
