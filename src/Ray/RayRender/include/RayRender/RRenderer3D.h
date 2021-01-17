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

		virtual void Draw(Scene& scene) = 0;
		virtual void ClearBuffer(float red, float green, float blue) = 0;
		virtual void EndDraw() = 0;

		virtual void* GetNativeWindow() const = 0;

	protected:
		Renderer3D() = default;
	};
}