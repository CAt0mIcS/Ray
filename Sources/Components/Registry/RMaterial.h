#pragma once

#include "../../RBase.h"
#include "../../Scene/REntity.h"
#include "../../Graphics/Images/RImage2D.h"


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;

	class RAY_EXPORT Material
	{
	public:
		Material(const Float4& baseDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f },
			std::shared_ptr<Image2D> imageDiffuse = nullptr, float metallic = 0.0f,
			float roughness = 0.0f, std::shared_ptr<Image2D> imageMaterial = nullptr,
			std::shared_ptr<Image2D> imageNormal = nullptr);
		~Material() {}

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;

	private:
		Entity m_Entity;
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;
	};


}  // namespace At0::Ray
