#pragma once

#include "../RMaterial.h"
#include "RayBase/RMath.h"

#include "Ray/Graphics/Pipelines/RGraphicsPipeline.h"

#include <optional>


namespace At0::Ray
{
	class Texture2D;

	/**
	 * Material which has all basic properties like
	 * Color, AlbedoMap, NormalMap, Metallic/SpecularMap, ...
	 */
	class RAY_EXPORT PhongMaterial : public Material
	{
	public:
		/**
		 * NOTES:
		 *	* If the diffuse map is nullptr, the metallic will be used for reflections.
		 *	* If both albedo map and color are specified, they will be interpolated. If the color is
		 *		white, it'll look like only the albedo map is active.
		 */
		struct Layout
		{
			Ref<Texture2D> albedoMap;
			Ref<Texture2D> diffuseMap;
			Ref<Texture2D> specularMap;
			Ref<Texture2D> normalMap;
			Ref<Texture2D> heightMap;
			Ref<Texture2D> occlusion;
			Ref<Texture2D> detailMask;
			Float4 color{};

			float metallic = 1.0f;

			struct
			{
				bool enabled = false;
				Float3 color{};
				float intensity = 0.0f;
			} emission{};
		};

	public:
#ifdef _MSC_VER
		PhongMaterial(Layout layout = {}, GraphicsPipeline::Layout pipelineLayout = {});
#else
		PhongMaterial(
			Layout layout = Layout{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
				{ 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, { false, { 0.0f, 0.0f, 0.0f }, 0.0f } },
			GraphicsPipeline::Layout pipelineLayout = GraphicsPipeline::Layout{});
#endif


		void SetColor(Float4 color) { m_Layout.color = std::move(color); }
		void SetMetallic(float metallic) { m_Layout.metallic = metallic; }
		void SetEmissionColor(Float3 color) { m_Layout.emission.color = std::move(color); }
		void SetEmissionIntensity(float intensity) { m_Layout.emission.intensity = intensity; }

		Ref<Texture2D> GetAlbedoMap() const override { return m_Layout.albedoMap; }
		Ref<Texture2D> GetDiffuseMap() const override { return m_Layout.diffuseMap; }
		Ref<Texture2D> GetSpecularMap() const override { return m_Layout.specularMap; }
		Ref<Texture2D> GetNormalMap() const override { return m_Layout.normalMap; }
		Ref<Texture2D> GetHeightMap() const override { return m_Layout.heightMap; }
		Ref<Texture2D> GetOcclusion() const override { return m_Layout.occlusion; }
		Ref<Texture2D> GetDetailMask() const override { return m_Layout.detailMask; }

	private:
		std::vector<std::string> ChooseShaders();

	private:
		Layout m_Layout;
	};
}  // namespace At0::Ray
