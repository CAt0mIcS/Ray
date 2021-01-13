#pragma once

#include "../RRBase.h"
#include "../Core/REntity.h"


namespace At0::Ray
{
	class RR_API Quad
	{
	public:
		template<typename T, typename... Args>
		decltype(auto) AddComponent(Args&&... args)
		{
			m_Entity.AddComponent<T>(std::forward<Args>(args)...);
		}

	private:
		Entity m_Entity;
	};
}