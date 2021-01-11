#pragma once


namespace At0::Ray
{
	class RendererAPI
	{
	public:
		enum API
		{
			D3D11, OpenGL,

			FIRST = D3D11, LAST = OpenGL
		};

		static API GetAPI() { return s_API; }
		static void SetAPI(API newAPI) { s_API = newAPI; }

	private:
		static API s_API;
	};

#ifdef _WIN32
	inline RendererAPI::API RendererAPI::s_API = RendererAPI::D3D11;
#elif defined(__linux__)
	inline RendererAPI::API RendererAPI::s_API = RendererAPI::OpenGL;
#else
#error "Invalid Platform"
#endif
}