#pragma once

#include "../../RRBase.h"
#include "../../RRenderer3D.h"

#include <../../RayEvent/include/RayEvent/REventListener.h>


struct GLFWwindow;

namespace At0::Ray
{
	class RR_API OpenGLRenderer3D : public Renderer3D,
		EventListener<WindowResizeEvent>
	{
	public:
		OpenGLRenderer3D(GLFWwindow* hWnd, EventDispatcher<WindowResizeEvent>& resizeDispatcher);
		~OpenGLRenderer3D();

		virtual void Draw(Scene& scene) override;
		virtual void ClearBuffer(float red, float green, float blue) override;
		virtual void EndDraw() override;

		virtual void* GetNativeWindow() const { return (void*)m_hWnd; }

		/// <summary>
		/// Handles viewport resizing
		/// </summary>
		virtual void OnEvent(Widget& receiver, WindowResizeEvent& e) override;

	private:
		GLFWwindow* m_hWnd;

		static bool s_GladInitialized;
	};
}
