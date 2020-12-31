#pragma once

#include "RInternal.h"
#include "REntityStorage.h"


namespace At0::Ray::ECS
{
	template<typename Component>
	struct ComponentStorage : EntityStorage
	{
	public:
		template<typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			if constexpr (std::is_aggregate_v<Component>)
				m_Instances.push_back(Component{ std::forward<Args>(args)... });
			else
				m_Instances.emplace_back(std::forward<Args>(args)...);

			EntityStorage::Emplace(entity, (IndexType)m_Instances.size() - 1);

			if constexpr (!std::is_empty_v<Component>)
				return m_Instances.back();
		}

		void Remove(Entity entity)
		{
			m_Instances.erase(EntityStorage::Index(entity));
			EntityStorage::Remove(entity);
		}

		const Component& Get(Entity entity) const
		{
			return m_Instances[EntityStorage::Index(entity)];
		}

		Component& Get(Entity entity)
		{
			return const_cast<Component&>(std::as_const(*this).Get(entity));
		}

		bool Has(Entity e) const
		{
			return EntityStorage::Has(e);
		}

	private:
		std::vector<Component> m_Instances;
	};
}

