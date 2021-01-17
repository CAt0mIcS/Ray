#pragma once

#include "../../RRBase.h"
#include "../../RRenderer3D.h"


struct GLFWwindow;

namespace At0::Ray
{
	class RR_API OpenGLRenderer3D : public Renderer3D
	{
	public:
		OpenGLRenderer3D(GLFWwindow* hWnd);
		~OpenGLRenderer3D();

		virtual void Draw(Scene& scene) override;
		virtual void ClearBuffer(float red, float green, float blue) override;
		virtual void EndDraw() override;

		virtual void* GetNativeWindow() const { return (void*)m_hWnd; }

	private:
		GLFWwindow* m_hWnd;

		static bool s_GLFWInitialized;
	};
}
