#include "RMeshContainer.h"


namespace At0::Ray
{
	void MeshContainer::AddMesh(Mesh mesh, Ref<Material> material)
	{
		Entity e = mesh.GetEntity();

		m_Meshes.emplace_back(
			MeshContainer::Data{ std::move(mesh), MeshRenderer{ e, std::move(material) } });
		RAY_DEBUG_FLAG(m_Name += "|" + std::string(m_Meshes.back().mesh.GetName()));
	}

	void MeshContainer::Render(const CommandBuffer& cmdBuff) const
	{
		for (const auto& [mesh, meshRenderer] : m_Meshes)
		{
			meshRenderer.Render(cmdBuff);
			mesh.CmdBind(cmdBuff);
		}
	}

	void MeshContainer::Update()
	{
		for (Data& data : m_Meshes)
			data.meshRenderer.Update();
	}
}  // namespace At0::Ray