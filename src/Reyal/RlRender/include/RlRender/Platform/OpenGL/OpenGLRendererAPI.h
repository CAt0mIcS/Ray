#ifndef RLR_OPENGLRENDERERAPI_H
#define RLR_OPENGLRENDERERAPI_H

#include "RlRender/RendererAPI.h"


namespace At0::Reyal
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		/// <summary>
		/// OpenGLRendererAPI Constructor
		/// </summary>
		OpenGLRendererAPI();

		/// <summary>
		/// Initializes all components required for rendering
		/// <param name="window">Is the platform specific window handle</param>
		/// </summary>
		virtual void Init(void* window) override;

		/// <summary>
		/// Checks if the renderer API was initialized
		/// </summary>
		/// <returns>True if initialized, false otherwise</returns>
		virtual bool IsInitialized() const override;

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		virtual void RenderTestTriangle() override;

		/// <summary>
		/// Draws the scene to the screen
		/// </summary>
		virtual void EndDraw() override;

	};
}


#endif // RLR_OPENGLRENDERERAPI_H