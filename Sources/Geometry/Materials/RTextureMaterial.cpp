#include "Rpch.h"
#include "RTextureMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	TextureMaterial::TextureMaterial(const Material::Config& config)
		: Material(
			  { "Resources/Shaders/TexturedShader.vert", "Resources/Shaders/TexturedShader.frag" },
			  nullptr, config.cullMode, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, config.polygonMode,
			  config.lineWidth),
		  m_Texture(config.texture2D)
	{
		AddUniform("texSampler", MakeScope<SamplerUniform>("texSampler", Shader::Stage::Fragment,
									 *m_Texture, GetGraphicsPipeline()));
	}
}  // namespace At0::Ray
