#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	Graphics::~Graphics() {}

	Graphics& Graphics::Get()
	{
		static Graphics graphics;
		return graphics;
	}

	Graphics::Graphics() {}
}  // namespace At0::Ray
