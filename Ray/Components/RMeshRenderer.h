#pragma once

#include "../RBase.h"
#include "RComponent.h"
#include "Base/RRenderer.h"

#include "../Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../Graphics/Pipelines/Uniforms/RSampler2DUniform.h"

#include <vector>
#include <unordered_map>


namespace At0::Ray
{
	class CommandBuffer;
	class Material;
	class Texture2D;
	enum class ShaderStage;

	/**
	 * Components containing all resources specifically required for a mesh to render.
	 * A mesh is only rendered if the contained entity has a MeshRenderer component
	 */
	class RAY_EXPORT MeshRenderer : public Component, public Renderer
	{
	public:
		/**
		 * @param automaticUniformEmplacement Controls whether predefined uniform tags should be
		 * used to add the uniforms. If false, all uniforms except for PerObjectData need to be
		 * added
		 */
		MeshRenderer(
			Entity entity, Ref<Material> material, bool automaticUniformEmplacement = true);

		/**
		 * Binds all of the mesh's resources
		 */
		void Render(const CommandBuffer& cmdBuff) const;

		/**
		 * Updates the predefined and required PerObjectData uniform's Model component to the
		 * current transform in the transform component
		 */
		void Update();

	private:
		/**
		 * Points to the buffer uniform in the unordered_map to make MeshRenderer::Update faster
		 */
		std::optional<BufferUniform::AccessType> m_PerObjectDataUniformRef;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(MeshRenderer);
