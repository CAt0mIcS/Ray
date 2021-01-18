#pragma once

#include "../RRBase.h"

#include "../Primitives/RDrawable.h"
#include "../Core/REntity.h"

#include <string_view>


namespace At0::Ray
{
	class Scene;

	class RR_API Quad : public DrawableBase<Quad>
	{
	public:
		/// <summary>
		/// Sets up buffers/... for drawing and adds TagComponent and TransformComponent
		/// to the Drawable Quad
		/// </summary>
		Quad(Entity entity);
	};
}