#pragma once

#include "../RBase.h"
#include "RTime.h"


namespace At0::Ray
{
	class RAY_EXPORT Engine
	{
	public:
		/**
		 * @returns The static application instance
		 */
		static Engine& Get();

		/**
		 * Starts the application run loop
		 * @returns the exit code
		 */
		int Run();

		/**
		 * @returns the time since the last frame
		 */
		Time GetDelta() const;

		/**
		 * First thing updated for now
		 */
		virtual void Update() = 0;

	protected:
		Engine();

	private:
		static Engine* s_Instance;
		Delta m_Delta;
	};
}  // namespace At0::Ray
