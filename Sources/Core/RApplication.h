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

		/**
		* Starts the application run loop
		* @returns the exit code
		*/
		int Run();

	protected:
		Application() = default;

	private:

	};
}