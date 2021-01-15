#pragma once

#include "../RRBase.h"

#include "../Primitives/RDrawable.h"
#include "../Core/REntity.h"

#include <string_view>


namespace At0::Ray
{
	class Scene;

	class RR_API Triangle : public Drawable
	{
	public:
		/// <summary>
		/// Sets up buffers/... for drawing and adds TagComponent and TransformComponent
		/// to the Drawable Quad
		/// </summary>
		Triangle(Entity entity);

	private:

	};
}