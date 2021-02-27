#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;

	class RAY_EXPORT Material
	{
	public:
		Material(Ref<GraphicsPipeline> pipeline);
		virtual ~Material() = default;

		const VertexLayout& GetVertexLayout() const;
		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }

		static Material Default();

	protected:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
	};
}  // namespace At0::Ray
