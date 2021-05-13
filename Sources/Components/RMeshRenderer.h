#pragma once

#include "../RBase.h"
#include "RComponent.h"


namespace At0::Ray
{
	class CommandBuffer;
	class Material;

	/**
	 * Components containing all resources specifically required for a mesh to render.
	 * A mesh is only rendered if the contained entity has a MeshRenderer component
	 */
	class RAY_EXPORT MeshRenderer : public Component
	{
	public:
		MeshRenderer(Entity entity);

		/**
		 * Binds all of the mesh's resources
		 */
		void Render(const CommandBuffer& cmdBuff) const;

		void SetMaterial(Ref<Material> material) { m_Material = std::move(material); }

		Material& GetMaterial() { return *m_Material; }
		const Material& GetMaterial() const { return *m_Material; }

	private:
		/**
		 * The shared material.
		 * NOTE: The material can be shared as it's practically just a bunch of data like the
		 * metallic or the specular map
		 */
		Ref<Material> m_Material;
	};
}  // namespace At0::Ray
