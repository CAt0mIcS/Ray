#include "RShadowMappingObjects.h"

#include "../RenderPass/RAttachment.h"
#include "../RenderPass/RRenderPass.h"
#include "../Buffers/RFramebuffer.h"
#include "../Images/RTexture.h"
#include "../Images/RImageView.h"
#include "../Pipelines/RGraphicsPipeline.h"
#include "../Pipelines/Shader/DataAccess/RDescriptor.h"
#include "../Pipelines/Shader/DataAccess/RBufferUniform.h"

#include "../RGraphics.h"
#include "../Core/RPhysicalDevice.h"
#include "../Pipelines/Shader/RShader.h"
#include "Scene/RScene.h"
#include "Scene/REntity.h"
#include "Components/RPointLight.h"
#include "Components/RTransform.h"
#include "../Commands/RCommandBuffer.h"

#define SHADOWMAP_DIM 4096
#define DEPTH_FORMAT VK_FORMAT_D16_UNORM

constexpr float depthBiasConstant = 5.f;
constexpr float depthBiasSlope = 1.75f;


namespace At0::Ray
{
	ShadowMappingObjects::ShadowMappingObjects()
	{
		// RenderPass
		{
			depthAttachment =
				MakeScope<Attachment>(DEPTH_FORMAT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
					Attachment::LoadOp::Clear, Attachment::StoreOp::Store,
					Attachment::LoadOp::Undefined, Attachment::StoreOp::Undefined);

			subpass.AddDepthAttachment(0, *depthAttachment);

			std::vector<VkSubpassDependency> dependencies(2);
			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			renderPass =
				MakeScope<RenderPass>(std::vector<VkAttachmentDescription>{ *depthAttachment },
					std::vector<VkSubpassDescription>{ subpass }, dependencies);
		}


		// Framebuffer
		{
			framebufferImage =
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

			framebuffer = MakeScope<Framebuffer>(*renderPass,
				std::vector<VkImageView>{ framebufferImage->GetImageView() },
				UInt2{ SHADOWMAP_DIM });
		}


		// Pipeline/offDescriptor/offUniform
		{
			pipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::FromSourceFile({ "Resources/Shaders/Offscreen.vert" }))
					.SetRenderPass(*renderPass)
					.SetDynamicStates({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
						VK_DYNAMIC_STATE_DEPTH_BIAS })
					.Build();

			uniform = MakeScope<BufferUniform>("UBO", ShaderStage::Vertex, *pipeline);
			(*uniform)["depthMVP"] = CalculateDepthModelViewProjectionMatrix();

			// auto meshRendererView =
			// Scene::Get().GetRegistry().group<MeshRenderer>(entt::get<Mesh>); for (uint32_t i = 0;
			// i < meshRendererView.size(); ++i)
			//{
			//	descriptors[(uint32_t)meshRendererView[i]] = MakeScope<DescriptorSet>(*pipeline, 1);
			//	descriptors[(uint32_t)meshRendererView[i]]->BindUniform(*uniform);
			//	descriptors[(uint32_t)meshRendererView[i]]->BindUniform(
			//		Entity{ meshRendererView[i] }.Get<MeshRenderer>().GetBufferUniform(
			//			"PerObjectData"));
			//}
		}
	}

	ShadowMappingObjects::~ShadowMappingObjects() {}

	Matrix ShadowMappingObjects::CalculateDepthModelViewProjectionMatrix() const
	{
		static Matrix depthProjectionMatrix = glm::perspective(Radians(60.f), 1.0f, 1.f, 96.f);

		// RAY_TODO: Disabling this due to scene not yet existing for now and instead using
		// placeholder empty vector auto ptLightView = Scene::Get().EntityView<PointLight>();
		std::vector<Entity> ptLightView{};

		if (ptLightView.size() == 0)
			return MatrixIdentity();

		// RAY_TODO: Multiple point lights
		Matrix depthViewMatrix =
			glm::lookAt(Entity{ ptLightView[0] }.Get<Transform>().Translation(), Float3{ 0.f },
				Float3{ 0.f, 1.f, 0.f });

		return depthProjectionMatrix * depthViewMatrix;
	}

	void ShadowMappingObjects::Draw(const CommandBuffer& cmdBuff,
		const entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Mesh>, MeshRenderer>&
			meshRendererView)
	{
		// RAY_TODO: Currently checking on every draw if we have new meshes added to scene
		// RAY_TODO: Use events!
		for (uint32_t i = 0; i < meshRendererView.size(); ++i)
		{
			if (!descriptors[(uint32_t)meshRendererView[i]])
			{
				descriptors[(uint32_t)meshRendererView[i]] = MakeScope<DescriptorSet>(*pipeline, 1);
				descriptors[(uint32_t)meshRendererView[i]]->BindUniform(*uniform);
				descriptors[(uint32_t)meshRendererView[i]]->BindUniform(
					Entity{ meshRendererView[i] }.Get<MeshRenderer>().GetBufferUniform(
						"PerObjectData"));
			}
		}

		VkClearValue clearValues;
		clearValues.depthStencil = { 1.f, 0 };

		renderPass->Begin(cmdBuff, *framebuffer, &clearValues, 1, { SHADOWMAP_DIM, SHADOWMAP_DIM });

		VkViewport viewport{};
		viewport.width = SHADOWMAP_DIM;
		viewport.height = SHADOWMAP_DIM;
		viewport.maxDepth = 1.f;
		vkCmdSetViewport(cmdBuff, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.extent = { SHADOWMAP_DIM, SHADOWMAP_DIM };
		vkCmdSetScissor(cmdBuff, 0, 1, &scissor);

		vkCmdSetDepthBias(cmdBuff, depthBiasConstant, 0.0f, depthBiasSlope);

		pipeline->CmdBind(cmdBuff);
		for (uint32_t i = 0; i < meshRendererView.size(); ++i)
		{
			const auto& mesh = meshRendererView.get<Mesh>(meshRendererView[i]);
			descriptors.at((uint32_t)meshRendererView[i])->CmdBind(cmdBuff);
			mesh.CmdBind(cmdBuff);
		}

		renderPass->End(cmdBuff);
	}
}  // namespace At0::Ray