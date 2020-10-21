#pragma once

#include "RlBase.h"
#include <wrl.h>

// TODO: Implement something


namespace Zeal::Reyal
{
	class RL_API Renderer
	{
	public:
		Renderer() = default;

	protected:
		static Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	};
}



