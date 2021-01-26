#include "RayRender/RRpch.h"

#include "RVulkanRenderer3D.h"
#include "Internal/RVulkanDevice.h"

#include "../../RVertexBuffer.h"
#include "../../RIndexBuffer.h"
#include "../../RShader.h"
#include "../../RTopology.h"
#include "../../RConstantBuffers.h"

#include <RayDebug/RAssert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RComponents.h>
#include "RVulkanRenderer3D.h"


namespace At0::Ray
{
	VulkanRenderer3D::VulkanRenderer3D(GLFWwindow* window, EventDispatcher<WindowResizeEvent>& resizeDispatcher)
		: VulkanGraphicsResources(window), m_hWnd{ window },
		EventListener<WindowResizeEvent>(resizeDispatcher)
	{
	}

	void VulkanRenderer3D::EndDraw()
	{
		GetDevice().Draw();
	}

	void VulkanRenderer3D::OnEvent(Widget& receiver, WindowResizeEvent& e)
	{
	}
}
