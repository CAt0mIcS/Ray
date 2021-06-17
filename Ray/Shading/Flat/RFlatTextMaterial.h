#pragma once

#include "../RMaterial.h"
#include "../../Core/RMath.h"
#include "../../Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	class RAY_EXPORT FlatTextMaterial : public Material
	{
	public:
		struct Layout
		{
			Float3 color;
		};

	public:
		FlatTextMaterial(Layout layout, GraphicsPipeline::Layout pipelineLayout);
	};
}  // namespace At0::Ray
