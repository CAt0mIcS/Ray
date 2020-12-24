#pragma once

#include "RInternal.h"
#include "RStorage.h"
#include "RView.h"

#include <utility>
#include <memory>

// RAY_TODO: Remove when Registry::Create implementation is finished.
#include <stdexcept>


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

		Entity Create()
		{
			Entity e;

			if (m_Destroyed == EntityNull)
			{
				// No entity to recycle
				e = m_Entities.emplace_back((uint32_t)m_Entities.size());
			}
			else
			{
				throw std::runtime_error("Missing implementation");
			}

			return e;
		}

		template<typename... Component>
		ComponentView<Component...> View()
		{
			return { Assure<Component>()... };
		}

		template<typename... Component>
		decltype(auto) Get(Entity e)
		{
			if constexpr (sizeof...(Component) == 1)
				return Assure<Component...>().Get(e);
			else
				return std::forward_as_tuple(Assure<Component>().Get(e)...);
		}

		template<typename... Component>
		decltype(auto) Get(Entity e) const
		{
			if constexpr (sizeof...(Component) == 1)
				return Assure<Component...>().Get(e);
			else
				return std::forward_as_tuple(Assure<Component>().Get(e)...);
		}

		template<typename... Component>
		bool Has(Entity e) const
		{
			return (Assure<Component>().Contains(e) && ...);
		}

	private:
		template<typename Component>
		struct PoolHandler : Storage<Component>
		{
			template<typename... Args>
			decltype(auto) Emplace(Entity e, Args&&... args)
			{
				Storage<Component>::Emplace(e, std::forward<Args>(args)...);

				if constexpr (!std::is_empty_v<Component>)
					return Storage<Component>::Get(e);
			}

			Component& Get(Entity e)
			{
				return Storage<Component>::Get(e);
			}
		};

		struct PoolData
		{
			uint32_t TypeID{};
			std::unique_ptr<SparseSet> Pool{};
		};

	private:
		template<typename Component>
		const PoolHandler<Component>& Assure() const
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

		template<typename Component>
		PoolHandler<Component>& Assure()
		{
			return const_cast<PoolHandler<Component>&>(std::as_const(*this).Assure<Component>());
		}

	private:
		mutable std::vector<PoolData> m_Pools;
		std::vector<Entity> m_Entities;
		Entity m_Destroyed{ EntityNull };
	};
}

