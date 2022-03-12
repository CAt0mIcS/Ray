#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RScriptableEntity.h>
#include <Ray/Components/RPointLight.h>
#include <Ray/Components/RModel.h>
#include <Ray/Components/RHierachyComponent.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Shading/RMaterial.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/Utils/RImGui.h>
#include <../../Extern/imgui/imgui.h>

#include <Ray/Graphics/RenderPass/RAttachment.h>
#include <Ray/Graphics/RenderPass/RRenderPass.h>
#include <Ray/Graphics/RenderPass/RSubpass.h>
#include <Ray/Graphics/Buffers/RFramebuffer.h>
#include <Ray/Graphics/Images/RImageView.h>
#include <Ray/Graphics/Images/RImage.h>
#include <Ray/Graphics/Pipelines/Shader/DataAccess/RDescriptor.h>
#include <Ray/Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h>
#include <Ray/Graphics/Commands/RCommandBuffer.h>
#include <Ray/Graphics/Images/RImage.h>
#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Utils/RGeometricPrimitives.h>
#include <Ray/Graphics/Core/RPhysicalDevice.h>
#include <Ray/Graphics/Core/RSwapchain.h>


using namespace At0::Ray;


class Scene2 : public Scene
{
public:
	Scene2() : Scene(MakeScope<Camera>())
	{
		UInt2 size = Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
};

#define SHADOWMAP_DIM 4096
#define DEPTH_FORMAT VK_FORMAT_D16_UNORM

constexpr float depthBiasConstant = 5.f;
constexpr float depthBiasSlope = 1.75f;


Scope<Attachment> offDepthAttachment;
Subpass offSubpass;
std::vector<VkSubpassDependency> offDependencies(2);
Scope<RenderPass> offRenderPass;
Scope<Framebuffer> offFramebuffer;
Ref<Texture> offFramebufferImage;
Ref<GraphicsPipeline> offPipeline;
std::unordered_map<uint32_t, Scope<DescriptorSet>> offDescriptors;
Scope<BufferUniform> offUniform;

Ref<GraphicsPipeline> debugPipeline;
Ref<Material> debugMaterial;
DescriptorSet* debugDescriptor;
Entity debugEntity;


class App : public Engine
{
public:
	App()
	{
		Scene::Create<Scene2>();

#include "../ImGuiWindows.inl"

		{
			auto shadedPipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::AcquireSourceFile({ "Resources/Shaders/ShadowMapping.vert",
						"Resources/Shaders/ShadowMapping.frag" }))
					.SetCullMode(VK_CULL_MODE_NONE)
					.Acquire();

			m_Material = Material::Builder(shadedPipeline).Build();

			m_Floor = Scene::Get().CreateEntity();
			m_Floor.Emplace<Model>("Resources/Models/Plane.obj", m_Material);
			m_Floor.Get<Mesh>().SetName("Floor");
			m_Floor.Get<Transform>().SetScale(Float3{ 6.f });

			m_Scene = Scene::Get().CreateEntity();
			m_Scene.Emplace<Model>("Resources/Models/SmoothVase.obj", m_Material);
			m_Scene.Get<Transform>()
				.SetTranslation(Float3{ 0.f, .4f, 0.f })
				.SetScale(Float3{ 2.f })
				.SetRotation(Float3{ Math::PI<>, 0.f, 0.f });

			auto flatColorPipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::AcquireSourceFile(
						{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
					.Acquire();

			auto flatWhiteMaterial =
				Material::Builder(flatColorPipeline).Set("Shading.color", Float4{ 1.f }).Acquire();

			m_Light = Scene::Get().CreateEntity();
			m_Light.Emplace<PointLight>().SetTranslation({ 5.f, 5.f, 0.f });
			m_Light.Emplace<Mesh>(Mesh::UVSphere(flatWhiteMaterial, .1f, 24, 24)).SetName("Light");
		}


		Matrix depthMVP = CalcDepthMVP();

		// Render pass
		{
			offDepthAttachment =
				MakeScope<Attachment>(DEPTH_FORMAT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
					Attachment::LoadOp::Clear, Attachment::StoreOp::Store,
					Attachment::LoadOp::Undefined, Attachment::StoreOp::Undefined);

			offSubpass.AddDepthAttachment(0, *offDepthAttachment);

			offDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			offDependencies[0].dstSubpass = 0;
			offDependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			offDependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			offDependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			offDependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			offDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			offDependencies[1].srcSubpass = 0;
			offDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			offDependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			offDependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			offDependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			offDependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			offDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			offRenderPass =
				MakeScope<RenderPass>(std::vector<VkAttachmentDescription>{ *offDepthAttachment },
					std::vector<VkSubpassDescription>{ offSubpass }, offDependencies);
		}

		// Framebuffer
		{
			offFramebufferImage =
				Texture::Builder()
					.SetImageType(VK_IMAGE_TYPE_2D)
					.SetExtent(UInt2{ SHADOWMAP_DIM })
					.SetMipLevels(1)
					.SetImageTiling(VK_IMAGE_TILING_OPTIMAL)
					.SetFormat(DEPTH_FORMAT)
					.SetImageUsage(
						VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
					.SetMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
					.SetImageAspect(VK_IMAGE_ASPECT_DEPTH_BIT)
					.SetTextureSampler(
						TextureSampler::Builder()
							.SetMinFilter(
								Graphics::Get().GetPhysicalDevice().IsFormatLinearlyFilterable(
									DEPTH_FORMAT, VK_IMAGE_TILING_OPTIMAL) ?
									  VK_FILTER_LINEAR :
									  VK_FILTER_NEAREST)
							.SetMagFilter(
								Graphics::Get().GetPhysicalDevice().IsFormatLinearlyFilterable(
									DEPTH_FORMAT, VK_IMAGE_TILING_OPTIMAL) ?
									  VK_FILTER_LINEAR :
									  VK_FILTER_NEAREST)
							.SetMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR)
							.SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
							.SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
							.SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
							.SetBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE)
							.BuildScoped())
					.Build();

			offFramebuffer = MakeScope<Framebuffer>(*offRenderPass,
				std::vector<VkImageView>{ offFramebufferImage->GetImageView() },
				UInt2{ SHADOWMAP_DIM });
		}

		// Pipeline/offDescriptor/offUniform
		{
			offPipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::AcquireSourceFile({ "Resources/Shaders/Offscreen.vert" }))
					.SetRenderPass(*offRenderPass)
					.SetDynamicStates({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
						VK_DYNAMIC_STATE_DEPTH_BIAS })
					.Build();

			offUniform = MakeScope<BufferUniform>("UBO", ShaderStage::Vertex, *offPipeline);
			(*offUniform)["depthMVP"] = depthMVP;

			auto meshRendererView = Scene::Get().GetRegistry().group<MeshRenderer>(entt::get<Mesh>);
			for (uint32_t i = 0; i < meshRendererView.size(); ++i)
			{
				offDescriptors[(uint32_t)meshRendererView[i]] =
					MakeScope<DescriptorSet>(*offPipeline, 1);
				offDescriptors[(uint32_t)meshRendererView[i]]->BindUniform(*offUniform);
				offDescriptors[(uint32_t)meshRendererView[i]]->BindUniform(
					Entity{ meshRendererView[i] }.Get<MeshRenderer>().GetBufferUniform(
						"PerObjectData"));
			}
		}

		// debug quad offPipeline/offDescriptor/offUniform
		{
			debugPipeline = GraphicsPipeline::Builder()
								.SetShader(Shader::AcquireSourceFile(
									{ "Tests/ShadowMapping/Shaders/DebugQuad.vert",
										"Tests/ShadowMapping/Shaders/DebugQuad.frag" }))
								.SetCullMode(VK_CULL_MODE_NONE)
								.SetVertexInputAttributeDescriptions({})
								.SetVertexInputBindingDescriptions({})
								.Build();

			debugMaterial = Material::Builder(debugPipeline).Build();

			debugEntity = Scene::Get().CreateEntity();
			debugEntity.Emplace<Mesh>(
				Mesh::Data{ nullptr, nullptr, debugMaterial, RAY_DEBUG_FLAG("DebugQuad") });

			debugDescriptor = &debugEntity.Get<MeshRenderer>().GetDescriptorSet(1);
		}


		Graphics::Get().OnCommandBufferRecord =
			[&](const CommandBuffer& cmdBuff, const Framebuffer& framebuffer, uint32_t imageIndex)
		{
			auto meshRendererView = Scene::Get().GetRegistry().group<MeshRenderer>(entt::get<Mesh>);

			VkClearValue clearValues[2];
			cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

			// First pass
			{
				clearValues[0].depthStencil = { 1.f, 0 };

				offRenderPass->Begin(
					cmdBuff, *offFramebuffer, clearValues, 1, { SHADOWMAP_DIM, SHADOWMAP_DIM });

				VkViewport viewport{};
				viewport.width = SHADOWMAP_DIM;
				viewport.height = SHADOWMAP_DIM;
				viewport.maxDepth = 1.f;
				vkCmdSetViewport(cmdBuff, 0, 1, &viewport);

				VkRect2D scissor{};
				scissor.extent = { SHADOWMAP_DIM, SHADOWMAP_DIM };
				vkCmdSetScissor(cmdBuff, 0, 1, &scissor);

				vkCmdSetDepthBias(cmdBuff, depthBiasConstant, 0.0f, depthBiasSlope);

				offPipeline->CmdBind(cmdBuff);
				// DrawShadowPass(m_Scene, cmdBuff);
				for (uint32_t i = 0; i < meshRendererView.size(); ++i)
				{
					const auto& mesh = meshRendererView.get<Mesh>(meshRendererView[i]);

					if (mesh.GetName() == "DebugQuad" || mesh.GetName() == "Light" ||
						mesh.GetName() == "Floor")
						continue;

					offDescriptors[(uint32_t)meshRendererView[i]]->CmdBind(cmdBuff);
					mesh.CmdBind(cmdBuff);
				}

				offRenderPass->End(cmdBuff);
			}


			// Seconds pass
			{

				// clearValues[0].color = { 0.0137254f, 0.014117f, 0.0149019f };
				clearValues[0].color = { 0.f, 0.f, 0.f };
				clearValues[1].depthStencil = { 1.0f, 0 };

				Graphics::Get().m_RenderPass->Begin(cmdBuff, framebuffer, clearValues,
					std::size(clearValues), Graphics::Get().GetSwapchain().GetExtent());

				vkCmdSetViewport(cmdBuff, 0, 1, &Graphics::Get().m_Viewport);
				vkCmdSetScissor(cmdBuff, 0, 1, &Graphics::Get().m_Scissor);

				// Visualize shadow map
				bool displayShadowMap = false;
				if (displayShadowMap)
				{
					debugDescriptor->CmdBind(cmdBuff);
					debugPipeline->CmdBind(cmdBuff);
					vkCmdDraw(cmdBuff, 3, 1, 0, 0);
				}

				for (uint32_t i = 0; i < meshRendererView.size(); ++i)
				{
					const auto& [meshRenderer, mesh] =
						meshRendererView.get<MeshRenderer, Mesh>(meshRendererView[i]);

					if (mesh.GetName() == "DebugQuad")
						continue;

					meshRenderer.Render(cmdBuff);
					mesh.CmdBind(cmdBuff);
				}


#if RAY_ENABLE_IMGUI
				ImGUI::Get().CmdBind(cmdBuff);
#endif

				Graphics::Get().m_RenderPass->End(cmdBuff);
			}

			cmdBuff.End();
		};


		m_Material->Set("Shading.lightSpace", depthMVP);
		m_Material->Set(
			"shadowMap", offFramebufferImage, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

		debugEntity.Get<MeshRenderer>().SetSamplerTexture(
			"shadowMap", offFramebufferImage, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}

private:
	void Update() override
	{
		Matrix rotateLight =
			glm::rotate(Matrix{ 1.f }, GetDelta().AsSeconds(), { 0.f, -5.5f, 0.f });
		m_Light.Get<PointLight>().SetTranslation(
			rotateLight * Float4{ m_Light.Get<Transform>().Translation(), 1.f });

		m_Material->Set("Shading.lightSpace", CalcDepthMVP());
		m_Material->Set(
			"Shading.lightPosition", Float4{ m_Light.Get<Transform>().Translation(), 1.f });
	}

private:
	Matrix CalcDepthMVP()
	{
		static Matrix depthProjectionMatrix = glm::perspective(Radians(60.f), 1.0f, 1.f, 96.f);

		Matrix depthViewMatrix = glm::lookAt(
			m_Light.Get<Transform>().Translation(), Float3{ 0.f }, Float3{ 0.f, 1.f, 0.f });

		// clang-format on

		return depthProjectionMatrix * depthViewMatrix;
	}

private:
	Entity m_Scene;
	Entity m_Floor;
	Entity m_Light;
	Ref<Material> m_Material;
};

void SignalHandler(int signal)
{
	Log::Critical("Signal {0} received", signal);
	Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Log::Open("Ray.log");
	Log::SetLogLevel(At0::Violent::LogLevel::Information);

	try
	{
		Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
		Window::Create();
		Window::Get().Show();
		Window::Get().SetTitle("SetupTest");

		return App{}.Run();
	}
	catch (Exception& e)
	{
		Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Log::Critical("Unknown exception occured.");
	}
}
