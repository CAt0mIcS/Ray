#include "Rpch.h"
#include "RSurface.h"

#include "Graphics/RGraphics.h"
#include "RRendererInstance.h"
#include "Devices/RWindow.h"


namespace At0::Ray
{
	Surface::Surface()
	{
		Window::Get().CreateSurface(Graphics::Get().GetInstance(), nullptr, &m_Surface);
	}

	Surface::~Surface() { vkDestroySurfaceKHR(Graphics::Get().GetInstance(), m_Surface, nullptr); }
}  // namespace At0::Ray
