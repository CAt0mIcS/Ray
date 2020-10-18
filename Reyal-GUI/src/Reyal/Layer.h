#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class Layer
	{
	public:
		Layer() = default;

		virtual void OnUpdate() = 0;
	};
}



