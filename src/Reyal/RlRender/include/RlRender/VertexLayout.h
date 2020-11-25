#pragma once

#include "RlRBase.h"


namespace At0::Reyal
{
	class RLR_API VertexLayout
	{
	public:
		static Ref<VertexLayout> Create();

	protected:
		VertexLayout() = default;
	};
}