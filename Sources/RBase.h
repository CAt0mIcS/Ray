#pragma once

#ifdef _MSC_VER
	#ifdef RAY_BUILD
		#define RAY_EXPORT __declspec(dllexport)
	#else
		#define RAY_EXPORT __declspec(dllimport)
	#endif
#else
	#define RAY_EXPORT
#endif

#define ENTT_API RAY_EXPORT

// Export some entt symbols. Specialization is not exported for some reason... (RAY_TODO)
#include <../../Extern/entt/src/entt/core/type_info.hpp>
#include <../../Extern/entt/src/entt/entity/storage.hpp>
#define RAY_EXPORT_COMPONENT(type)                                                                 \
	template<>                                                                                     \
	struct RAY_EXPORT entt::type_seq<At0::Ray::type>                                               \
	{                                                                                              \
		[[nodiscard]] static entt::id_type value() ENTT_NOEXCEPT                                   \
		{                                                                                          \
			static const entt::id_type value = entt::internal::type_seq::next();                   \
			return value;                                                                          \
		}                                                                                          \
	};                                                                                             \
                                                                                                   \
	template<>                                                                                     \
	struct RAY_EXPORT entt::type_seq<                                                              \
		entt::sigh_storage_mixin<                                                                  \
			entt::storage_adapter_mixin<entt::basic_storage<entt::entity, At0::Ray::type, void>>>, \
		void>                                                                                      \
	{                                                                                              \
		[[nodiscard]] static entt::id_type value() ENTT_NOEXCEPT                                   \
		{                                                                                          \
			static const entt::id_type value = entt::internal::type_seq::next();                   \
			return value;                                                                          \
		}                                                                                          \
	};


#define RAY_ENABLE_IMGUI 1


#include <memory>

namespace At0::Ray
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
}  // namespace At0::Ray
