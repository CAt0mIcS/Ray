#pragma once

#include "RStorage.h"

#include <tuple>
#include <type_traits>


namespace At0::Ray::ECS
{
	template<typename... Component>
	class ComponentView
	{
	private:
		template<typename Comp>
		using PoolType = std::conditional_t<std::is_const_v<Comp>, const Storage<std::remove_const_t<Comp>>, Storage<Comp>>;

	public:
		ComponentView(Storage<Component>&... component)
			: m_Components{ &component... }
		{

		}

	private:
		std::tuple<PoolType<Component>*...> m_Components;
	};
}