#pragma once

#include "RInternal.h"
#include "RStorage.h"

#include <utility>
#include <memory>


namespace At0::Ray::ECS
{
	class Registry
	{
	public:
		template<typename Component, typename... Args>
		decltype(auto) Emplace(Entity entity, Args&&... args)
		{
			return Assure<Component>().Emplace(entity, std::forward<Args>(args)...);
		}

	private:
		template<typename Component>
		const Component& Assure() const
		{
			if constexpr (HasComponentIndex<Component>::value)
			{
				const auto index = ComponentIndex<Component>::Value();
				if (index >= m_Pools.size())
					m_Pools.resize(index + 1);

				if (auto&& poolData = m_Pools[index]; !poolData.Pool)
				{
					poolData.TypeID = ComponentIndex<Component>::Value();
					poolData.Pool.reset(new PoolHandler<Component>());
				}

				return (PoolHandler<Component>&)(*m_Pools[index].Pool);
			}
			else
			{
				SparseSet* candidate{ nullptr };

				if (auto it = std::find_if(m_Pools.begin(), m_Pools.end(), [id = ComponentIndex<Component>::Value()](const auto& poolData) { return id == poolData.TypeID; }); it == m_Pools.cend())
				{
					candidate = m_Pools.emplace_back(PoolData{
						ComponentIndex<Component>::Value(),
						std::unique_ptr<SparseSet>{new PoolHandler<Component>{}}
						}
					).Pool.get();
				}
				else
				{
					candidate = it->Pool.get();
				}

				return (PoolHandler<Component>&)(*candidate);
			}
		}

	private:
		template<typename Component>
		struct PoolHandler : Storage<Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity e, Args&&... args)
			{
				Storage<Component>::Emplace(e, std::forward<Args>(args)...);
			}

			const Component& Get(Entity e) const
			{
				return Storage<Component>::Get(e);
			}

			Component& Get(Entity e)
			{
				return const_cast<Component>(std::as_const(*this).Get(e));
			}
		};

		struct PoolData
		{
			uint32_t TypeID;
			std::unique_ptr<SparseSet> Pool{};
		};

	private:
		std::vector<PoolData> m_Pools;
		std::vector<Entity> m_Entities;
		Entity m_Destroyed{ EntityNull };
	};
}

