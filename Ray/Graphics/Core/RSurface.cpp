#include "Rpch.h"
#include "RSurface.h"

#include "Graphics/RGraphics.h"
#include "RRendererInstance.h"
#include "Devices/RWindow.h"

#include "Ray/Core/RRendererLoader.h"


namespace At0::Ray
{
	Surface::Surface() { Window::Get().CreateSurface(&m_Surface); }

	Surface::~Surface()
	{
		RendererAPI::DestroySurfaceKHR(Graphics::Get().GetInstance(), m_Surface);
	}
}  // namespace At0::Ray
