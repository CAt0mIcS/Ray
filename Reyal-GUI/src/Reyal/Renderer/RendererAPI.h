#pragma once

#include "RlBase.h"


namespace At0::Reyal
{
	class RendererAPI
	{
	public:
		/// <summary>
		/// Enum describing the current renderer API to use
		/// </summary>
		enum class API
		{
			None = 0, D3D11,

			FIRST = None, LAST = D3D11
		};

	public:
		/// <summary>
		/// Virtual RendererAPI Deconstructor
		/// </summary>
		virtual ~RendererAPI() = default;

		/// <summary>
		/// Initializes all components required for rendering
		/// <param name="window">Is the platform specific window handle</param>
		/// </summary>
		virtual void Init(_In_ void* window) = 0;

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		virtual void RenderTestTriangle() = 0;

		/// <summary>
		/// Getter for the currently used API
		/// </summary>
		/// <returns>The current API</returns>
		static API GetAPI() { return s_Api; }

		/// <summary>
		/// Setter for the API (TODO)
		/// </summary>
		/// <param name="newApi">Is the new rendering API to be used</param>
		static void SetAPI(API newApi) { s_Api = newApi; }

		/// <summary>
		/// Creates the API-Specific rendering API
		/// </summary>
		/// <returns></returns>
		static Scope<RendererAPI> Create();

	private:
		/// <summary>
		/// Specifies the current rendering API
		/// </summary>
		static API s_Api;
	};
}



