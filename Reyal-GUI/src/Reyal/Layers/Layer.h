#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class RL_API Layer
	{
	public:
		Layer() = default;

		virtual void OnUpdate() = 0;
	};
}



