#pragma once

#include "../RBase.h"
#include "Core/RSharedBindable.h"
#include "../Utils/RModel.h"
#include "Pipelines/Shader/RShader.h"
//#include "../Utils/RLogger.h"

#include <string>
#include <unordered_map>
#include <mutex>


namespace At0::Ray
{
	/**
	 * Handles static bindables and models by hashing them by tag
	 */
	class RAY_EXPORT Codex
	{
	public:
		/**
		 * Checks if a bindable was already created by a previous drawable.
		 * If not, the codex will create it
		 */
		template<typename T, typename... Args>
		static Ref<T> Resolve(Args&&... args)
		{
			return Get().BindableResolve<T>(std::forward<Args>(args)...);
		}

		/**
		 * Clears the codex map
		 */
		static void Shutdown() { Get().InternalShutdown(); }

	private:
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}

		template<typename T, typename... Args>
		Ref<T> BindableResolve(Args&&... args)
		{
			std::string tag = T::GetUID(args...);

			std::scoped_lock lock(m_BindableMutex);
			auto it = m_Bindables.find(tag);
			// Key does not exist, create it
			if (it == m_Bindables.end())
			{
				m_Bindables[tag] = MakeRef<T>(std::forward<Args>(args)...);
				// Log::Trace(
				//	"[Codex] SharedBindable (Tag=\"{0}\") was created because it didn't exist",
				//	tag);
				return std::static_pointer_cast<T>(m_Bindables[tag]);
			}
			// Key exists, return it
			else
			{
				// Log::Trace("[Codex] SharedBindable (Tag=\"{0}\") already exists", tag);
				return std::static_pointer_cast<T>(it->second);
			}
		}

		void InternalShutdown()
		{
			std::scoped_lock lockBindable(m_BindableMutex);
			m_Bindables.clear();
		}

	private:
		std::unordered_map<std::string, Ref<SharedBindable>> m_Bindables;
		std::mutex m_BindableMutex;
	};
}  // namespace At0::Ray
