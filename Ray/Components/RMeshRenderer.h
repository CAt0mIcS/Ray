#pragma once

#include "../RBase.h"
#include "RComponent.h"
#include "Base/RRenderer.h"

#include "../Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"
#include "../Shading/RMaterial.h"
#include "../Scene/RCamera.h"
#include "../Events/REventListener.h"

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
	class RAY_EXPORT MeshRenderer :
		public Component,
		public Renderer,
		EventListener<MaterialBecameDirtyEvent>,
		EventListener<CameraChangedEvent>
	{
	public:
		MeshRenderer(Entity entity, Ref<Material> material);

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

		void UpdateUniform(const std::string& dataPath, bool isPushConstant = false);

		virtual void OnEvent(MaterialBecameDirtyEvent& e) override;
		virtual void OnEvent(CameraChangedEvent& e) override { m_IsCameraDirty = true; }

	private:
		/**
		 * Points to the buffer uniform in the unordered_map to make MeshRenderer::Update faster
		 */
		std::optional<BufferUniform::AccessType> m_PerObjectDataUniformRef;

		/**
		 * RAY_TEMPORARY: Due to pipeline mismatches, we'll (for now) have a descriptor set for
		 * scene data in the mesh renderer as well
		 */
		BufferUniform* m_PerSceneUniform;
		mutable bool m_IsCameraDirty = true;

		RAY_DEBUG_FLAG(mutable std::string m_Name);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(MeshRenderer);
