#pragma once

#include "../../RBase.h"
#include "RImage2D.h"
#include "../../Core/RComponent.h"

#include <string>
#include <../../Extern/entt/src/entt/core/type_info.hpp>


namespace At0::Ray
{
	class RAY_EXPORT Texture : public Component, NonCopyable
	{
	public:
		Texture(Entity& entity);
		virtual ~Texture() = default;

		Texture& operator=(Texture&& other) = default;
		Texture(Texture&& other) = default;
	};
}  // namespace At0::Ray
