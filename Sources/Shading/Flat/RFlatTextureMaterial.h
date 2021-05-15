#pragma once

#include "../RMaterial.h"
#include "../../Graphics/Pipelines/RGraphicsPipeline.h"
#include "../../Graphics/Images/RTexture2D.h"


namespace At0::Ray
{
	class RAY_EXPORT FlatTextureMaterial : public Material
	{
	public:
		struct Layout
		{
			Ref<Texture2D> texture;
		};

	public:
		FlatTextureMaterial(Layout layout = {}, GraphicsPipeline::Layout pipelineLayout = {});

		Ref<Texture2D> GetDiffuseMap() const override { return m_Layout.texture; }

	private:
		std::vector<std::string> ChooseShaders();

	private:
		Layout m_Layout;
	};
}  // namespace At0::Ray
