#ifndef RL_RENDERER_H
#define RL_RENDERER_H

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


#endif /* RL_RENDERER_H */

