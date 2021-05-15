#pragma once

#include "../RMaterial.h"
#include "../../Core/RMath.h"

#include "../../Graphics/Pipelines/RGraphicsPipeline.h"


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
			} emission;
		};

	public:
		PhongMaterial(Layout layout = {}, GraphicsPipeline::Layout pipelineLayout = {});

		void SetColor(Float4 color) { m_Layout.color = std::move(color); }
		void SetMetallic(float metallic) { m_Layout.metallic = metallic; }
		void SetEmissionColor(Float3 color) { m_Layout.emission.color = std::move(color); }
		void SetEmissionIntensity(float intensity) { m_Layout.emission.intensity = intensity; }

	private:
		std::vector<std::string> ChooseShaders();

	private:
		Layout m_Layout;
	};
}  // namespace At0::Ray
