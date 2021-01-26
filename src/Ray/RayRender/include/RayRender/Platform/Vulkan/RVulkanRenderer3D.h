#pragma once

#include "../../RRBase.h"
#include "../../RRenderer3D.h"
#include "Internal/RVulkanGraphicsResources.h"

#include <../../RayEvent/include/RayEvent/REventListener.h>

struct GLFWwindow;

namespace At0::Ray
{
	class RR_API VulkanRenderer3D : public Renderer3D, VulkanGraphicsResources,
		EventListener<WindowResizeEvent>
	{
	public:
		VulkanRenderer3D(GLFWwindow* window, EventDispatcher<WindowResizeEvent>& resizeDispatcher);

		virtual void Draw(const Camera& camera, Scene& scene) override {}
		virtual void ClearBuffer(float red, float green, float blue) override {}
		virtual void EndDraw() override;

		virtual void* GetNativeWindow() const override { return 0; }

	private:
		void OnEvent(Widget& receiver, WindowResizeEvent& e);

	private:
		GLFWwindow* m_hWnd;
	};
}
