#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class RAY_EXPORT Material
	{
	public:
		virtual ~Material() = default;
	};


	class GraphicsPipeline;
	class RAY_EXPORT DefaultMaterial : public Material
	{
	public:
		DefaultMaterial() = default;
		~DefaultMaterial();

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
	};
}  // namespace At0::Ray
