#pragma once

#include "../RBase.h"

#if RAY_USE_IMGUI

// clang-format off
#include "../Events/REventListener.h"
#include "../Events/REngineEvents.h"

#include <vulkan/vulkan_core.h>
// clang-format on

struct ImGui_ImplVulkanH_Window;

namespace At0::Ray
{
	class ImGuiContext
	{
	public:
		ImGuiContext();
		~ImGuiContext();

		static ImGuiContext& Create();
		static void Destroy();
		static ImGuiContext& Get();

		void OnFramebufferResized();
		void NewFrame();

	private:
		void SetupVulkanWindow();
		void CreateDescriptorPool();
		void CreateRenderPass();
		void UploadFonts();

	private:
		static Scope<ImGuiContext> s_Instance;

		VkDescriptorPool m_DescriptorPool;
		VkRenderPass m_RenderPass;
		Scope<ImGui_ImplVulkanH_Window> m_WindowData;
	};
}  // namespace At0::Ray

#endif
