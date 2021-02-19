#pragma once

#include "../RBase.h"

#include <map>
#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Graphics
	{
	public:
		~Graphics();
		static Graphics& Get();

	private:
		Graphics();
	};
}  // namespace At0::Ray
