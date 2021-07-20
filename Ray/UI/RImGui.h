#pragma once

#ifndef RAY_ENABLE_IMGUI
	#define RAY_ENABLE_IMGUI 0
#endif
#ifndef RAY_ENABLE_IMGUI_DOCKSPACE
	#define RAY_ENABLE_IMGUI_DOCKSPACE 0
#endif


#if RAY_ENABLE_IMGUI

// clang-format off
#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"
#include "../Core/RTime.h"

#include "Ray/Events/REventListener.h"
#include "Ray/Events/RKeyboardEvents.h"
#include "Ray/Events/RMouseEvents.h"
#include "Ray/Events/REngineEvents.h"
#include <RayRenderer/Core/RImage.h>
#include <RayRenderer/Pipeline/RDescriptor.h>

#include <functional>
// clang-format on

typedef struct RrSampler_T* RrSampler;
typedef struct RrImageView_T* RrImageView;

namespace At0::Ray
{
	class Texture2D;
	class Buffer;
	class CommandBuffer;
	class GraphicsPipeline;
	class DescriptorSet;
	class Sampler2DUniform;

	/**
	 * RAY_TODO:
	 *	ImGui should be the first to receive events
	 */

	class RAY_EXPORT ImGUI :
		EventListener<FramebufferResizedEvent>,
		EventListener<MouseMovedEvent>,
		EventListener<MouseButtonPressedEvent>,
		EventListener<MouseButtonReleasedEvent>,
		EventListener<KeyPressedEvent>,
		EventListener<KeyReleasedEvent>,
		EventListener<CharEvent>,
		EventListener<ScrollLeftEvent>,
		EventListener<ScrollRightEvent>,
		EventListener<ScrollUpEvent>,
		EventListener<ScrollDownEvent>
	{
	public:
		struct PushConstBlock
		{
			Float2 scale;
			Float2 translate;
		} m_PushConstBlock;

	public:
		static ImGUI& Get();
		static void Destroy() { s_Instance.reset(); }
		~ImGUI();

		void NewFrame(std::function<void()> fn = []() {});
		void UpdateBuffers();
		void CmdBind(const CommandBuffer& cmdBuff);
		void Update(Delta dt);

		template<typename F>
		void RegisterNewFrameFunction(F&& func)
		{
			m_NewFrameFunctions.emplace_back(func);
		}

		static Float3 Float3Widget(std::string_view title, Float3 data);

		/**
		 * Should be called on stored textures to get the ID required for ImGui::Image
		 * @returns First parameter of ImGui::Image function
		 */
		void* PushTexture(const Texture2D& texture);

		/**
		 * Should be called on stored textures to get the ID required for ImGui::Image
		 * @returns First parameter of ImGui::Image function
		 */
		void* PushTexture(RrSampler sampler, RrImageView imageView, RrImageLayout imageLayout);

	private:
		ImGUI();

		void InitResources();
		void CreatePipeline();
		void CreateTextureUploadResources();

		void OnEvent(FramebufferResizedEvent& e) override;
		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(MouseButtonPressedEvent& e) override;
		void OnEvent(MouseButtonReleasedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;
		void OnEvent(CharEvent& e) override;
		void OnEvent(ScrollLeftEvent& e) override;
		void OnEvent(ScrollRightEvent& e) override;
		void OnEvent(ScrollUpEvent& e) override;
		void OnEvent(ScrollDownEvent& e) override;

	private:
		static Scope<ImGUI> s_Instance;

		Ref<Texture2D> m_FontImage;

		Scope<GraphicsPipeline> m_Pipeline;
		Scope<Sampler2DUniform> m_FontUniform;
		Scope<DescriptorSet> m_FontDescriptor;

		RrDescriptorSetLayout m_TextureDescriptorSetLayout;
		std::vector<RrDescriptorSet> m_TextureDescriptorSets;

		Scope<Buffer> m_VertexBuffer;
		Scope<Buffer> m_IndexBuffer;

		int32_t m_VertexCount = 0;
		int32_t m_IndexCount = 0;

		std::vector<std::function<void()>> m_NewFrameFunctions;
	};
}  // namespace At0::Ray

#endif
