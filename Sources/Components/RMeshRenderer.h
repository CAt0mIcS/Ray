#pragma once

#include "../RBase.h"
#include "RComponent.h"

#include "../Graphics/Pipelines/RShader.h"
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

	/**
	 * Predefined tags by which to find certain uniforms which always need to be there
	 */
	struct UniformTag
	{
		static constexpr const char* PerObjectData = "PerObjectData";
		static constexpr const char* Shading = "Shading";
		static constexpr const char* AlbedoMapSampler = "samplerAlbedo";
		static constexpr const char* DiffuseMapSampler = "samplerDiffuse";
		static constexpr const char* SpecularMapSampler = "samplerSpecular";
		static constexpr const char* NormalMapSampler = "samplerNormal";
	};


	/**
	 * Components containing all resources specifically required for a mesh to render.
	 * A mesh is only rendered if the contained entity has a MeshRenderer component
	 */
	class RAY_EXPORT MeshRenderer : public Component
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

		BufferUniform& AddBufferUniform(std::string_view name, Shader::Stage stage);
		Sampler2DUniform& AddSampler2DUniform(
			std::string_view name, Shader::Stage stage, Ref<Texture2D> texture);

		BufferUniform& GetBufferUniform(std::string_view name);
		Sampler2DUniform& GetSampler2DUniform(std::string_view name);

		void SetMaterial(Ref<Material> material);

		Material& GetMaterial() { return *m_Material; }
		const Material& GetMaterial() const { return *m_Material; }
		Ref<Material> GetSharedMaterial() const { return m_Material; }

	private:
		void AddUniforms();

	private:
		/**
		 * The shared material.
		 * NOTE: The material can be shared as it's practically just a bunch of data like the
		 * metallic or the specular map
		 */
		Ref<Material> m_Material;

		std::vector<DescriptorSet> m_DescriptorSets;
		std::unordered_map<uint32_t, std::vector<Scope<BufferUniform>>> m_BufferUniforms;
		std::unordered_map<uint32_t, std::vector<Scope<Sampler2DUniform>>> m_Sampler2DUniforms;

		/**
		 * Points to the buffer uniform in the unordered_map to make MeshRenderer::Update faster
		 */
		std::optional<BufferUniform::AccessType> m_PerObjectDataUniformRef;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(MeshRenderer);
