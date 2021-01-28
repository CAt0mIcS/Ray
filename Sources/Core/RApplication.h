#pragma once

#include "../RBase.h"

namespace At0::Ray
{
	class RAY_EXPORT Application
	{
	public:
		/**
		* @returns The static application instance
		*/
		static Application& Get();

	protected:
		Application() = default;

	private:

	};
}