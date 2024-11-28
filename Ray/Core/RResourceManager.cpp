#include "RResourceManager.h"


namespace At0::Ray
{
	ResourceManager& ResourceManager::Get()
	{
		static ResourceManager resources;
		return resources;
	}

	void ResourceManager::Destroy()
	{
		Get().m_Resources.clear();
	}
}  // namespace At0::Ray
