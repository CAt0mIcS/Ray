#pragma once

#include "../RBase.h"
#include "RComponent.h"
#include "Base/RRenderer.h"

#include "../Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../Graphics/Pipelines/Uniforms/RSampler2DUniform.h"
#include "../Graphics/Pipelines/Shader/RShaderReflection.h"

#include <vector>
#include <unordered_map>
#include <stdint.h>


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
		MeshRenderer(Entity entity, Ref<Material> material);
		//~MeshRenderer();

		// MeshRenderer(MeshRenderer&& other) noexcept;
		// MeshRenderer& operator=(MeshRenderer&& other) noexcept;

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
		 * Updates ALL uniforms with material data
		 */
		void SetMaterialData();

		/**
		 * Updates uniform with name with material data
		 */
		void UpdateMaterialData(const std::string& uniformPath, UniformType uType);

		void UpdateUniform(const std::string& dataPath);

	private:
		/**
		 * Points to the buffer uniform in the unordered_map to make MeshRenderer::Update faster
		 */
		std::optional<BufferUniform::AccessType> m_PerObjectDataUniformRef;

		uint32_t m_DirtyListenerIndex = (uint32_t)-1;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(MeshRenderer);
