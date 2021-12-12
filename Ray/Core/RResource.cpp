#include "RResource.h"


namespace At0::Ray
{
	Resources& Resources::Get()
	{
		static Resources resources;
		return resources;
	}

	void Resources::Destroy() { Get().m_Resources.clear(); }
}  // namespace At0::Ray
