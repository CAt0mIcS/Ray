#include "Rpch.h"
#include "RMeshRenderer.h"

#include "Graphics/RGraphics.h"
#include "Geometry/RMesh.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer() : EventListener<EntityCreatedEvent>(Scene::Get()) {}

	void MeshRenderer::Update(Delta ts) {}

	void MeshRenderer::Bind(const CommandBuffer& cmdBuff) const
	{
		auto meshView = Scene::Get().EntityView<Mesh>();
		meshView.each([&cmdBuff](Mesh& mesh) { mesh.Render(cmdBuff); });
	}

	void MeshRenderer::OnEvent(EntityCreatedEvent& e) { Graphics::Get().RerecordCommandBuffers(); }
}  // namespace At0::Ray
