#pragma once

#include "RInternal.h"
#include "RSparseSet.h"

#include <type_traits>
#include <utility>


namespace At0::Ray::ECS
{
	template<typename Component>
	class Storage : public SparseSet
	{
	public:
		template<typename... Args>
		decltype(auto) Emplace(Entity e, Args&&... args)
		{
			if constexpr (std::is_aggregate_v<Component>)
				m_Components.push_back(Component{ std::forward<Args>(args)... });
			else
				m_Components.emplace_back(std::forward<Args>(args)...);

			SparseSet::Emplace(e);
		}

		const Component& Get(Entity e) const
		{
			return m_Components[SparseSet::Index(e)];
		}

		Component& Get(Entity e)
		{
			return const_cast<Component&>(std::as_const(*this).Get(e));
		}

	private:
		std::vector<Component> m_Components;
	};
}