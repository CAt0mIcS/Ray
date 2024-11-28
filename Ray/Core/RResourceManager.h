#pragma once

#include "../RBase.h"

#include <mutex>
#include <string>
#include <type_traits>
#include <unordered_map>


namespace At0::Ray
{
	class RAY_EXPORT Resource
	{
	public:
		virtual ~Resource() = default;
	};

	// RAY_TODO: Check if thread-safe
	class RAY_EXPORT ResourceManager
	{
	public:
		static ResourceManager& Get();
		static void Destroy();

		template<typename T, typename... Args>
		Ref<T> EmplaceIfNonExistent(const std::string& tag, Args&&... args)
			requires(std::derived_from<T, Resource>, std::constructible_from<T, Args...>)
		{
			auto it = m_Resources.find(tag);
			if (it == m_Resources.end())
			{
				auto res = MakeRef<T>(std::forward<Args>(args)...);

				std::scoped_lock lock(m_ResourceMutex);
				m_Resources.emplace(tag, res);
				return res;
			}

			return std::static_pointer_cast<T>(it->second);
		}

		template<typename T>
		Ref<T> Emplace(const std::string& tag, Ref<T>&& resource)
		{
			std::scoped_lock lock(m_ResourceMutex);
			m_Resources.emplace(tag, resource);
			return resource;
		}

		Ref<Resource> operator[](const std::string& tag)
		{
			// RAY_TODO: Not required due to only access?
			// std::scoped_lock lock(m_ResourceMutex);
			auto it = m_Resources.find(tag);
			if (it == m_Resources.end())
				return nullptr;
			return it->second;
		}

		template<typename T>
		Ref<T> Get(const std::string& tag)
			requires std::derived_from<T, Resource>
		{
			return std::static_pointer_cast<T>(operator[](tag));
		}

	private:
		std::unordered_map<std::string, Ref<Resource>> m_Resources;
		std::mutex m_ResourceMutex;
	};
}  // namespace At0::Ray
