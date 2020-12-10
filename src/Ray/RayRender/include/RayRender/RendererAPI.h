#pragma once

#include "RlRBase.h"


namespace At0::Ray
{
	class RR_API RendererAPI
	{
	public:
		enum RR_API API
		{
			None = 0, OpenGL, D3D11,

			FIRST = None, LAST = D3D11
		};

	public:
		RendererAPI() = delete;
		
		static void SetAPI(API api) { s_API = api; }
		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}