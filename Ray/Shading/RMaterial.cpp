#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	void Material::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_GraphicsPipeline->CmdBind(cmdBuff);
	}

	Material::Material() {}
}  // namespace At0::Ray
