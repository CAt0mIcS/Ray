#include "Rpch.h"
#include "RMeshRenderer.h"

#include "../Shading/RMaterial.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer(Entity entity) : Component(entity) {}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const { m_Material->CmdBind(cmdBuff); }
}  // namespace At0::Ray
