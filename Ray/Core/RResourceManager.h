#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"

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

		template<typename T, typename... Args>
		Ref<T> EmplaceOrGet(const std::string& tag, Args&&... args)
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

		template<typename T, typename... Args>
		Ref<T> Emplace(const std::string& tag, Args&&... args)
			requires(std::derived_from<T, Resource>, std::constructible_from<T, Args...>)
		{
			return Emplace(tag, MakeRef<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		Ref<T> Emplace(const std::string& tag, Ref<T>&& resource)
			requires std::derived_from<T, Resource>
		{
			std::scoped_lock lock(m_ResourceMutex);
			RAY_MEXPECTS(m_Resources.find(tag) == m_Resources.end(),
				"[ResourceManager] Resource with tag {0} already exists", tag);
			m_Resources.emplace(tag, resource);
			return resource;
		}

		void Destroy(const std::string& tag);

		Ref<Resource> operator[](const std::string& tag);

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
