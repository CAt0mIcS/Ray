#include "RResourceManager.h"


namespace At0::Ray
{
	ResourceManager& ResourceManager::Get()
	{
		static ResourceManager resources;
		return resources;
	}

	void ResourceManager::Destroy(const std::string& tag)
	{
		std::scoped_lock lock(m_ResourceMutex);
		RAY_MEXPECTS(m_Resources.find(tag) != m_Resources.end(),
			"[ResourceManager] Resource with tag {0} not found", tag);
		m_Resources.erase(tag);
	}

	Ref<Resource> ResourceManager::operator[](const std::string& tag)
	{
		// RAY_TODO: Not required due to only access?
		// std::scoped_lock lock(m_ResourceMutex);
		auto it = m_Resources.find(tag);
		if (it == m_Resources.end())
			return nullptr;
		return it->second;
	}
}  // namespace At0::Ray
