#pragma once

#include "Ray/RBase.h"

#include <mutex>


namespace At0::Ray
{
	class RAY_EXPORT Resource
	{
	public:
		virtual ~Resource() = default;
	};

	// RAY_TODO: Check if thread-safe
	class RAY_EXPORT Resources
	{
	public:
		static Resources& Get();
		static void Destroy();

		template<typename T, typename... Args>
		Ref<T> EmplaceIfNonExistent(const std::string& tag, Args&&... args)
		{
			if (m_Resources.find(tag) == m_Resources.end())
			{
				auto res = MakeRef<T>(std::forward<Args>(args)...);

				std::scoped_lock lock(m_ResourceMutex);
				m_Resources.emplace(tag, res);
				return res;
			}

			return std::static_pointer_cast<T>(m_Resources.at(tag));
		}

	private:
		std::unordered_map<std::string, Ref<Resource>> m_Resources;
		std::mutex m_ResourceMutex;
	};
}  // namespace At0::Ray
