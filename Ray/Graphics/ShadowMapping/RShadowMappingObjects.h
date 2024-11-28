#pragma once

#include "../../RBase.h"

#include "../RenderPass/RSubpass.h"
#include "../../Core/RMath.h"
#include "../../Scene/REntity.h"
#include "../../Components/RMeshRenderer.h"
#include "../../Components/RMesh.h"


namespace At0::Ray
{
	class Attachment;
	class RenderPass;
	class Framebuffer;
	class Texture;
	class GraphicsPipeline;
	class DescriptorSet;
	class BufferUniform;
	class CommandBuffer;
	class RenderContext;

	struct RAY_EXPORT ShadowMappingObjects
	{
		ShadowMappingObjects(const RenderContext& context);
		~ShadowMappingObjects();

		Matrix CalculateDepthModelViewProjectionMatrix() const;
		void Draw(
			const CommandBuffer& cmdBuff, const entt::basic_group<entt::entity, entt::exclude_t<>,
											  entt::get_t<Mesh>, MeshRenderer>& meshRendererView);

		Scope<Attachment> depthAttachment;
		Subpass subpass;
		Scope<RenderPass> renderPass;
		Scope<Framebuffer> framebuffer;
		Ref<Texture> framebufferImage;
		Ref<GraphicsPipeline> pipeline;
		std::unordered_map<uint32_t, Scope<DescriptorSet>> descriptors;
		Scope<BufferUniform> uniform;

		const RenderContext& context;

		// Ref<GraphicsPipeline> debugPipeline;
		// Ref<Material> debugMaterial;
		// DescriptorSet* debugDescriptor;
		// Entity debugEntity;
	};
}  // namespace At0::Ray
