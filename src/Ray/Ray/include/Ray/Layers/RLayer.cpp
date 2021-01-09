#include "Ray/Rpch.h"
#include "Ray/Layers/RLayer.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>

#include "Ray/RApplication.h"
#include "Ray/Window/RWindow.h"


namespace At0::Ray
{
	Layer::Layer(std::string_view name)
		: m_Name(name)
	{
		RAY_PROFILE_FUNCTION();

	}

	Window& Layer::GetMainWindow()
	{
		return Ray::Application::Get().GetMainWindow();
	}

	const Window& Layer::GetMainWindow() const
	{
		return Ray::Application::Get().GetMainWindow();
	}

	Layer::~Layer()
	{
		Log::Debug("[Layer] '{0}' destroyed", m_Name);
	}
}

