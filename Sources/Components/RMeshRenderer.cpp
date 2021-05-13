#include "Rpch.h"
#include "RMeshRenderer.h"

namespace At0::Ray
{
	MeshRenderer::MeshRenderer(Entity entity) : Component(entity) {}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const {}
}  // namespace At0::Ray
