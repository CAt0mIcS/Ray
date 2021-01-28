#include "Rpch.h"
#include "RApplication.h"


namespace At0::Ray
{
	Application& Application::Get()
	{
		static Application instance;
		return instance;
	}
}