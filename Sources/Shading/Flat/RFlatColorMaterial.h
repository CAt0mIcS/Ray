#pragma once

#include "../RMaterial.h"
#include "../../Graphics/Pipelines/RGraphicsPipeline.h"
#include "../../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT FlatColorMaterial : public Material
	{
	public:
		struct Layout
		{
			Float4 color;
		};

	public:
		FlatColorMaterial(Layout layout = {}, GraphicsPipeline::Layout pipelineLayout = {});
		Float4 GetColor() const override { return m_Layout.color; }

	private:
		std::vector<std::string> ChooseShaders();

	private:
		Layout m_Layout;
	};
}  // namespace At0::Ray
