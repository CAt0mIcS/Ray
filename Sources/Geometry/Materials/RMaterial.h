#pragma once

#include "../../RBase.h"
#include "../../Graphics/Core/RBindable.h"
#include "../../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../../Graphics/Pipelines/Uniforms/RSamplerUniform.h"
#include "../../Graphics/Images/RTexture2D.h"

#include <type_traits>
#include <vector>


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;


	class RAY_EXPORT Material : public Bindable
	{
	public:
		struct CullMode
		{
			CullMode(VkCullModeFlags val) : value(val) {}
			operator VkCullModeFlags() const { return value; }

			VkCullModeFlags value;
		};

		struct PolygonMode
		{
			PolygonMode(VkPolygonMode val) : value(val) {}
			operator VkPolygonMode() const { return value; }

			VkPolygonMode value;
		};

		struct LineWidth
		{
			LineWidth(float val) : value(val) {}
			operator float() const { return value; }

			float value;
		};

		struct Color
		{
			Color(Float3 val) : value(std::move(val)) {}
			operator const Float3&() const { return value; }

			Float3 value;
		};

		struct SpecularMap
		{
			SpecularMap(Ref<Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Texture2D>&() const { return value; }

			Ref<Texture2D> value;
		};

		struct DiffuseMap
		{
			DiffuseMap(Ref<Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Texture2D>&() const { return value; }

			Ref<Texture2D> value;
		};

		struct NormalMap
		{
			NormalMap(Ref<Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Texture2D>&() const { return value; }

			Ref<Texture2D> value;
		};

		struct LightTechnique
		{
			enum Technique
			{
				Flat,
				Phong
			};

			LightTechnique(Technique val) : value(val) {}
			operator Technique() const { return value; }

			Technique value;
		};

	protected:
		struct Config
		{
			CullMode cullMode = VK_CULL_MODE_BACK_BIT;
			PolygonMode polygonMode = VK_POLYGON_MODE_FILL;
			LineWidth lineWidth = 1.0f;
			Color color = { { 1.0f, 1.0f, 1.0f } };
			SpecularMap specularMap = { nullptr };
			DiffuseMap diffuseMap = { nullptr };
			NormalMap normalMap = { nullptr };
			Ref<Texture2D> texture2D = nullptr;
			LightTechnique lightTechnique = LightTechnique::Phong;
		};

	public:
		virtual ~Material();

		virtual const Texture2D* GetDiffuseMap() const { return nullptr; }
		virtual const Texture2D* GetSpecularMap() const { return nullptr; }
		virtual const Texture2D* GetNormalMap() const { return nullptr; }

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;

		void CmdBind(const CommandBuffer& cmdBuff) const override;

		/**
		 * Adds a uniform to the list of uniforms which are automatically bound in CmdBind
		 * @param tag Unique tag to identify the uniform
		 * @param uniform Uniform to add
		 */
		Uniform& AddUniform(std::string_view tag, Scope<Uniform> uniform);

		/**
		 * @returns If the uniform with tag has been added
		 */
		bool HasUniform(std::string_view tag) const;

		/**
		 * @param tag The tag used when adding the uniform with "AddUniform"
		 * @returns The uniform which was added using "AddUniform"
		 */
		Uniform& GetUniform(std::string_view tag);

		Material& operator=(Material&& other) noexcept;
		Material(Material&& other) noexcept;

	protected:
		Material(const std::vector<std::string>& shaders, VertexLayout* pVertexLayout = nullptr,
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f);
		Material() = default;

		void CreatePipeline(const std::vector<std::string>& shaders,
			VertexLayout* pVertexLayout = nullptr, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f);

		static void FillConfig(Config& config, CullMode cullMode) { config.cullMode = cullMode; }
		static void FillConfig(Config& config, PolygonMode polygonMode);
		static void FillConfig(Config& config, LineWidth lineWidth);
		static void FillConfig(Config& config, Color color) { config.color = color; }
		static void FillConfig(Config& config, SpecularMap spec) { config.specularMap = spec; }
		static void FillConfig(Config& config, DiffuseMap diff) { config.diffuseMap = diff; }
		static void FillConfig(Config& config, NormalMap norm) { config.normalMap = norm; }
		static void FillConfig(Config& config, Ref<Texture2D> tex) { config.texture2D = tex; }
		static void FillConfig(Config& config, LightTechnique tec) { config.lightTechnique = tec; }

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;
		std::vector<Scope<Uniform>> m_Uniforms;
	};


}  // namespace At0::Ray
