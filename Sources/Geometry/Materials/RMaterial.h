#pragma once

#include "../../RBase.h"
#include "../../Graphics/Core/RBindable.h"
#include "../../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../../Graphics/Pipelines/Uniforms/RSamplerUniform.h"

#include <type_traits>
#include <vector>


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;
	class Texture2D;


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

	protected:
		struct Config
		{
			CullMode cullMode = VK_CULL_MODE_BACK_BIT;
			PolygonMode polygonMode = VK_POLYGON_MODE_FILL;
			LineWidth lineWidth = 1.0f;
			Color color = { { 1.0f, 1.0f, 1.0f } };
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
		Material(std::vector<std::string> shaders,
			const Float4& baseDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f },
			Ref<Texture2D> diffuseMap = nullptr, Ref<Texture2D> specularMap = nullptr,
			Ref<Texture2D> normalMap = nullptr, float metallic = 0.0f, float roughness = 0.0f,
			VertexLayout* pVertexLayout = nullptr, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f);

		static void FillConfig(Config& config, CullMode cullMode) { config.cullMode = cullMode; }
		static void FillConfig(Config& config, PolygonMode polygonMode);
		static void FillConfig(Config& config, LineWidth lineWidth);
		static void FillConfig(Config& config, Color color) { config.color = color; }


	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;
		std::vector<Scope<Uniform>> m_Uniforms;
	};


}  // namespace At0::Ray
