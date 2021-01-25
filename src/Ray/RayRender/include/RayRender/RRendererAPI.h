#pragma once

#include "RRBase.h"


namespace At0::Ray
{
	class RR_API RendererAPI
	{
	public:
		enum API
		{
			D3D11, OpenGL, Vulkan,

			FIRST = D3D11, LAST = Vulkan
		};

		static API GetAPI();
		static void SetAPI(API newAPI);

		static bool Valid();

	private:
		static API s_API;
	};
}