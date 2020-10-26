#pragma once

#include "RlBase.h"
#include <wrl.h>

// TODO: Implement something


namespace At0::Reyal
{
	class RL_API Renderer
	{
	public:
		Renderer() = default;

	protected:
		static Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	};
}



