#include "Rpch.h"
#include "RFlatMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	FlatMaterial::FlatMaterial(const Material::Config& config)
		: Material({ "Resources/Shaders/FlatShader.vert", "Resources/Shaders/FlatShader.frag" },
			  { 1.0f, 1.0f, 1.0f, 1.0f }, nullptr, nullptr, nullptr, 0.0f, 0.0f, nullptr,
			  config.cullMode, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, config.polygonMode,
			  config.lineWidth),
		  m_Color(config.color)
	{
		BufferUniform& uColor = (BufferUniform&)AddUniform("Color",
			MakeScope<BufferUniform>("Color", Shader::Stage::Fragment, GetGraphicsPipeline()));

		uColor["color"] = m_Color;
	}
}  // namespace At0::Ray
