#include "Rpch.h"
#include "RColorMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	ColorMaterial::ColorMaterial(const Material::Config& config)
		: Material({ "Resources/Shaders/FlatShader.vert", "Resources/Shaders/FlatShader.frag" },
			  nullptr, config.cullMode, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, config.polygonMode,
			  config.lineWidth),
		  m_Color(config.color)
	{
		BufferUniform& uColor = (BufferUniform&)AddUniform("Color",
			MakeScope<BufferUniform>("Color", Shader::Stage::Fragment, GetGraphicsPipeline()));

		uColor["color"] = m_Color;
	}
}  // namespace At0::Ray
