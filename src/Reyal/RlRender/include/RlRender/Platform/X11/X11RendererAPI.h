#ifndef RLR_X11RENDERERAPI_H
#define RLR_X11RENDERERAPI_H


#ifdef __linux__


#include "RlRender/RendererAPI.h"


namespace At0::Reyal
{
    class X11RendererAPI : public RendererAPI
    {
    public:
        /// <summary>
		/// X11RendererAPI Constructor
		/// </summary>
        X11RendererAPI();

        /// <summary>
		/// Virtual RendererAPI Deconstructor
		/// </summary>
		virtual ~X11RendererAPI();

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

#endif // __linux__


#endif // RLR_X11RENDERERAPI_H