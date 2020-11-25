#pragma once


namespace At0::Reyal
{
	/// <summary>
	/// Same as D3D11RenderBase
	/// </summary>
	class OpenGLGraphicsResource
	{
	protected:
		OpenGLGraphicsResource() = default;

	protected:
		static uint32_t s_RendererID;
	};
}


