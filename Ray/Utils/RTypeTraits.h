#pragma once

#include <type_traits>


namespace At0::Ray
{
	template<typename T1, typename T2>
	constexpr bool IsSameType = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T1>>,
		std::remove_cv_t<std::remove_reference_t<T2>>>;
}