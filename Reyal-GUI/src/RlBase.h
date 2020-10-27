#ifndef RL_RLBASE_H
#define RL_RLBASE_H

#include <memory>


#ifdef RL_BUILD
	#define RL_API __declspec(dllexport)
#else
	#define RL_API __declspec(dllimport)
#endif

namespace At0::Reyal
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif /* RL_RLBASE_H */
