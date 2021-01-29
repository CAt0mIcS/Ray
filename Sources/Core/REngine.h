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
		Time GetDelta() const { return m_Delta.Change(); }

		/**
		 * @returns The FPS
		 */
		uint32_t GetFPS() const { return m_FPS.Value(); }

		/**
		 * First thing updated for now
		 */
		virtual void Update() = 0;

	protected:
		Engine();

	private:
		static Engine* s_Instance;
		Delta m_Delta;
		ChangePerSecond m_FPS;
	};
}  // namespace At0::Ray
