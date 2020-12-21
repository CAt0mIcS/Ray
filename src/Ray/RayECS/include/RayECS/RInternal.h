#pragma once

#include <stdint.h>
#include <type_traits>


namespace At0::Ray::ECS::Internal
{
	struct ComponentIndex
	{
		static const uint32_t Next()
		{
			static uint32_t value = 0;
			return value++;
		}
	};
}


namespace At0::Ray::ECS
{
	inline constexpr uint32_t EntityNull = -1;
	using Entity = uint32_t;

	template<typename Component>
	struct ComponentIndex
	{
		static const uint32_t Value()
		{
			static const uint32_t next = Internal::ComponentIndex::Next();
			return next;
		}
	};


	template<typename, typename = void>
	struct HasComponentIndex : std::false_type {};

	template<typename Component>
	struct HasComponentIndex<Component, std::void_t<decltype(ComponentIndex<Component>::Value())>> : std::true_type {};

}