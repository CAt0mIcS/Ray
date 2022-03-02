#pragma once

#include "RComponent.h"

#include "RMesh.h"
#include "RMeshRenderer.h"


namespace At0::Ray
{
	class CommandBuffer;
	class Material;

	/**
	 * Allows one entity to have multiple meshes, should only be used when importing models
	 * (RAY_TODO)
	 */
	class RAY_EXPORT MeshContainer : public Component
	{
	public:
		MeshContainer(Entity entity) : Component(entity) {}

		void AddMesh(Mesh mesh, Ref<Material> material);

		RAY_DEBUG_FLAG(std::string_view GetName() const { return m_Name; })

		void Render(const CommandBuffer& cmdBuff) const;
		void Update();

	private:
		struct Data
		{
			Mesh mesh;
			MeshRenderer meshRenderer;
		};

		std::vector<Data> m_Meshes;
		RAY_DEBUG_FLAG(std::string m_Name);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(MeshContainer);
