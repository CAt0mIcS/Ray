#pragma once

#include "RlBase.h"
#include "RendererAPI.h"


namespace At0::Reyal
{
	class RL_API Renderer3D
	{
	public:
		/// <summary>
		/// Renderer3D Constructor
		/// </summary>
		Renderer3D();

		/// <summary>
		/// Initializes Renderer aand RendererAPI
		/// </summary>
		/// <param name="window">Is the platform specific window to render to</param>
		void Init(_In_ void* window);

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		void RenderTestTriangle();

		/// <summary>
		/// Draws the scene to the screen
		/// </summary>
		void EndDraw();

	private:
		/// <summary>
		/// Rendering calls will be dispatched to the platform's API
		/// </summary>
		Scope<RendererAPI> m_API;
	};

}


