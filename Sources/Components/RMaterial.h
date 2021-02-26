#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;

	class RAY_EXPORT Material
	{
	public:
		virtual ~Material() = default;

		const VertexLayout& GetVertexLayout() const;

		static Material Default();

	protected:
		Material(Ref<GraphicsPipeline> pipeline);

	protected:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
	};
}  // namespace At0::Ray
