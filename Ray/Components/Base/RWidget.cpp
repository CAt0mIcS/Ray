#include "RWidget.h"

#include "Scene/REntity.h"
#include "Components/RTransform.h"
#include "Components/RMesh.h"

#include "Devices/RWindow.h"


namespace At0::Ray
{
	void Widget::SetTranslation(Float2 coords)
	{
		SetX(coords.x);
		SetY(coords.y);
	}

	void Widget::SetX(float x)
	{
		m_Translation.x = x;

		Transform& tform = GetEntity().Get<Transform>();
		tform.SetTranslation({ ScreenSpaceToNDCSpaceX(m_Translation.x + (GetScale().x / 2.0f),
								   Window::Get().GetFramebufferSize().x),
			tform.Translation().y, 0.0f });
	}

	void Widget::SetY(float y)
	{
		m_Translation.y = y;

		Transform& tform = GetEntity().Get<Transform>();
		tform.SetTranslation({ tform.Translation().x,
			ScreenSpaceToNDCSpaceY(
				m_Translation.y + (GetScale().y / 2.0f), Window::Get().GetFramebufferSize().y),
			0.0f });
	}

	void Widget::SetScale(Float2 scale)
	{
		SetWidth(scale.x);
		SetHeight(scale.y);
	}

	void Widget::SetWidth(float width)
	{
		m_Width = width;

		Transform& tform = GetEntity().Get<Transform>();
		Float3 newScale{ m_Width / (0.5f * Window::Get().GetFramebufferSize().x), tform.Scale().y,
			1.0f };
		tform.SetScale(newScale);
		SetX(GetX());
	}

	void Widget::SetHeight(float height)
	{
		m_Height = height;

		Transform& tform = GetEntity().Get<Transform>();
		Float3 newScale{ tform.Scale().x, m_Height / (0.5f * Window::Get().GetFramebufferSize().y),
			1.0f };
		tform.SetScale(newScale);
		SetY(GetY());
	}

	Widget::Widget(Entity entity, std::string_view name)
		: EventListener<WindowResizedEvent>(Window::Get()), Component(entity), m_Name(name)
	{
	}

	void Widget::OnEvent(WindowResizedEvent& e)
	{
		// Anchor mesh to stay at the same position whenever the window is resized

		Transform& tform = GetEntity().Get<Transform>();
		SetWidth(m_Width);
		SetHeight(m_Height);

		Ray::Float2 scale = tform.Scale();

		float newX =
			(m_Translation.x / (float)Ray::Window::Get().GetFramebufferSize().x) * 2.0f - 1;
		float newY =
			(m_Translation.y / (float)Ray::Window::Get().GetFramebufferSize().y) * 2.0f - 1;

		newX += scale.x / 2.0f;
		newY += scale.y / 2.0f;

		tform.SetTranslation({ newX, newY, 0.0f });
	}
}  // namespace At0::Ray
