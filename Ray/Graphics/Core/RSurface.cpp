#include "Rpch.h"
#include "RSurface.h"

#include "Graphics/RGraphics.h"
#include "RRendererInstance.h"
#include "Devices/RWindow.h"

#include "Core/RRendererLoader.h"


namespace At0::Ray
{
	Surface::Surface() { Window::Get().CreateSurface(&m_Surface); }

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(Graphics::Get().GetInstance(), (VkSurfaceKHR)m_Surface, nullptr);
	}
}  // namespace At0::Ray
