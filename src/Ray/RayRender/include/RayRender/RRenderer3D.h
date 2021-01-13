#pragma once

#include "RRBase.h"

#include "Core/RScene.h"


namespace At0::Ray
{
	class RR_API Renderer3D
	{
	public:
		static Ref<Renderer3D> Create(void* window);

		virtual ~Renderer3D() = default;

		virtual void Draw(const Scene& scene) = 0;
	protected:
		Renderer3D() = default;
	};
}