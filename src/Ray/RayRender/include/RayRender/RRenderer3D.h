#pragma once

#include "RRBase.h"

namespace At0::Ray
{
	class RR_API Renderer3D
	{
	public:
		static Ref<Renderer3D> Create();

	protected:
		Renderer3D() = default;
	};
}